/*
 * uloop - event loop implementation
 *
 * Copyright (C) 2010-2016 Felix Fietkau <nbd@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <sys/time.h>
#include <sys/types.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <poll.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

#include "uloop.h"
#include "utils.h"

#ifdef USE_KQUEUE
#include <sys/event.h>
#endif
#ifdef USE_EPOLL
#include <sys/epoll.h>
#endif
#include <sys/wait.h>

struct uloop_fd_event {
	struct uloop_fd *fd;
	unsigned int events;
};

struct uloop_fd_stack {
	struct uloop_fd_stack *next;
	struct uloop_fd *fd;
	unsigned int events;
};

static struct uloop_fd_stack *fd_stack = NULL;

#define ULOOP_MAX_EVENTS 10

static struct list_head timeouts = LIST_HEAD_INIT(timeouts);
static struct list_head processes = LIST_HEAD_INIT(processes);

static int poll_fd = -1;
bool uloop_cancelled = false;
bool uloop_handle_sigchld = true;
static int uloop_status = 0;
static bool do_sigchld = false;

static struct uloop_fd_event cur_fds[ULOOP_MAX_EVENTS];
static int cur_fd, cur_nfds;
static int uloop_run_depth = 0;

int uloop_fd_add(struct uloop_fd *sock, unsigned int flags);

#ifdef USE_KQUEUE
#include "uloop-kqueue.c"
#endif

#ifdef USE_EPOLL
#include "uloop-epoll.c"
#endif

static void waker_consume(struct uloop_fd *fd, unsigned int events)
{
	char buf[4];

	while (read(fd->fd, buf, 4) > 0)
		;
}

static int waker_pipe = -1;
static struct uloop_fd waker_fd = {
	.fd = -1,
	.cb = waker_consume,
};

static void waker_init_fd(int fd)
{
	fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

static int waker_init(void)
{
	int fds[2];

	if (waker_pipe >= 0)
		return 0;

	if (pipe(fds) < 0)
		return -1;

	waker_init_fd(fds[0]);
	waker_init_fd(fds[1]);
	waker_pipe = fds[1];

	waker_fd.fd = fds[0];
	waker_fd.cb = waker_consume;
	uloop_fd_add(&waker_fd, ULOOP_READ);

	return 0;
}

static void uloop_setup_signals(bool add);

int uloop_init(void)
{
	if (uloop_init_pollfd() < 0)
		return -1;

	if (waker_init() < 0) {
		uloop_done();
		return -1;
	}

	uloop_setup_signals(true);

	return 0;
}

static bool uloop_fd_stack_event(struct uloop_fd *fd, int events)
{
	struct uloop_fd_stack *cur;

	/*
	 * Do not buffer events for level-triggered fds, they will keep firing.
	 * Caller needs to take care of recursion issues.
	 */
	if (!(fd->flags & ULOOP_EDGE_TRIGGER))
		return false;

	for (cur = fd_stack; cur; cur = cur->next) {
		if (cur->fd != fd)
			continue;

		if (events < 0)
			cur->fd = NULL;
		else
			cur->events |= events | ULOOP_EVENT_BUFFERED;

		return true;
	}

	return false;
}

static void uloop_run_events(int timeout)
{
	struct uloop_fd_event *cur;
	struct uloop_fd *fd;

	if (!cur_nfds) {
		cur_fd = 0;
		cur_nfds = uloop_fetch_events(timeout);
		if (cur_nfds < 0)
			cur_nfds = 0;
	}

	while (cur_nfds > 0) {
		struct uloop_fd_stack stack_cur;
		unsigned int events;

		cur = &cur_fds[cur_fd++];
		cur_nfds--;

		fd = cur->fd;
		events = cur->events;
		if (!fd)
			continue;

		if (!fd->cb)
			continue;

		if (uloop_fd_stack_event(fd, cur->events))
			continue;

		stack_cur.next = fd_stack;
		stack_cur.fd = fd;
		fd_stack = &stack_cur;
		do {
			stack_cur.events = 0;
			fd->cb(fd, events);
			events = stack_cur.events & ULOOP_EVENT_MASK;
		} while (stack_cur.fd && events);
		fd_stack = stack_cur.next;

		return;
	}
}

int uloop_fd_add(struct uloop_fd *sock, unsigned int flags)
{
	unsigned int fl;
	int ret;

	if (!(flags & (ULOOP_READ | ULOOP_WRITE)))
		return uloop_fd_delete(sock);

	if (!sock->registered && !(flags & ULOOP_BLOCKING)) {
		fl = fcntl(sock->fd, F_GETFL, 0);
		fl |= O_NONBLOCK;
		fcntl(sock->fd, F_SETFL, fl);
	}

	ret = register_poll(sock, flags);
	if (ret < 0)
		goto out;

	sock->registered = true;
	sock->eof = false;
	sock->error = false;

out:
	return ret;
}

int uloop_fd_delete(struct uloop_fd *fd)
{
	int i;

	for (i = 0; i < cur_nfds; i++) {
		if (cur_fds[cur_fd + i].fd != fd)
			continue;

		cur_fds[cur_fd + i].fd = NULL;
	}

	if (!fd->registered)
		return 0;

	fd->registered = false;
	uloop_fd_stack_event(fd, -1);
	return __uloop_fd_delete(fd);
}

static int tv_diff(struct timeval *t1, struct timeval *t2)
{
	return
		(t1->tv_sec - t2->tv_sec) * 1000 +
		(t1->tv_usec - t2->tv_usec) / 1000;
}

int uloop_timeout_add(struct uloop_timeout *timeout)
{
	struct uloop_timeout *tmp;
	struct list_head *h = &timeouts;

	if (timeout->pending)
		return -1;

	list_for_each_entry(tmp, &timeouts, list) {
		if (tv_diff(&tmp->time, &timeout->time) > 0) {
			h = &tmp->list;
			break;
		}
	}

	list_add_tail(&timeout->list, h);
	timeout->pending = true;

	return 0;
}

/*
 * 获取系统当前时间
 */
static void uloop_gettime(struct timeval *tv)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	tv->tv_sec = ts.tv_sec;
	tv->tv_usec = ts.tv_nsec / 1000;
}

/*
 * 设置超时时间
 */
int uloop_timeout_set(struct uloop_timeout *timeout, int msecs)
{
	struct timeval *time = &timeout->time;

	if (timeout->pending)
		uloop_timeout_cancel(timeout);

	uloop_gettime(time);

	time->tv_sec += msecs / 1000;
	time->tv_usec += (msecs % 1000) * 1000;

	if (time->tv_usec > 1000000) {
		time->tv_sec++;
		time->tv_usec -= 1000000;
	}

    /* 添加到 timeout 列表，按照超时时间由小到大的顺序排列 */
	return uloop_timeout_add(timeout);
}

int uloop_timeout_cancel(struct uloop_timeout *timeout)
{
	if (!timeout->pending)
		return -1;

	list_del(&timeout->list);
	timeout->pending = false;

	return 0;
}

int uloop_timeout_remaining(struct uloop_timeout *timeout)
{
	struct timeval now;

	if (!timeout->pending)
		return -1;

	uloop_gettime(&now);

	return tv_diff(&timeout->time, &now);
}

/*
 * 将当前 uloop_process 结构加入到 processes 双向列表，
 * 按照 pid 由小到大的顺序排列；最后置 p->pending 标记位为 true
 */
int uloop_process_add(struct uloop_process *p)
{
	struct uloop_process *tmp;
	struct list_head *h = &processes;

	if (p->pending) // 已经加入 processes 链表
		return -1;

    /* 找出第一个比 p->pid 大的节点 */
	list_for_each_entry(tmp, &processes, list) {
		if (tmp->pid > p->pid) {
			h = &tmp->list;
			break;
		}
	}

	list_add_tail(&p->list, h); // 将 p->list 插入到 h 前面
	p->pending = true;

	return 0;
}

int uloop_process_delete(struct uloop_process *p)
{
	if (!p->pending)
		return -1;

	list_del(&p->list);
	p->pending = false;

	return 0;
}

static void uloop_handle_processes(void)
{
	struct uloop_process *p, *tmp;
	pid_t pid;
	int ret;

    /* 首先将 do_sigchld 设置为 false，等待下次信号产生再激活该函数 */
	do_sigchld = false;

    /* 调用 waitpid 来获取已经结束的子进程的 PID，并通过该 pid 在 processes
     * 链表内寻找对应节点，从链表中删除并执行其回调函数 */
	while (1) {
		pid = waitpid(-1, &ret, WNOHANG);
		if (pid < 0 && errno == EINTR)
			continue;

		if (pid <= 0) // 状态没变, 由于设置了 WNOHANG，因此立即返回
			return;

        /* 由于 pid 按小到大排列，因此只有相等的才执行删除并调用回调函数 */
		list_for_each_entry_safe(p, tmp, &processes, list) {
			if (p->pid < pid) // 小于 pid，还没找到
				continue;

			if (p->pid > pid) // 大于 pid，已经过了
				break;

            // 相等才执行以下的删除节点和回调函数
			uloop_process_delete(p);
			p->cb(p, ret);
		}
	}

}

static void uloop_signal_wake(void)
{
	do {
		if (write(waker_pipe, "w", 1) < 0) {
			if (errno == EINTR)
				continue;
		}
		break;
	} while (1);
}

/*
 * 将全局变量 uloop_cancelled 设置为 true
 */
static void uloop_handle_sigint(int signo)
{
	uloop_status = signo;
	uloop_cancelled = true;
	uloop_signal_wake();
}

/*
 * 将全局变量 do_sigchld 设置为 true，当子进程停止或终止时，
 * 将 SIGCHLD 信号发送给其父进程，系统默认忽略此信号
 */
static void uloop_sigchld(int signo)
{
	do_sigchld = true;
	uloop_signal_wake();
}

/*
 * 设置信号 signum 的处理函数
 * 若 add == true，并且信号原来的处理函数是默认处理函数，则更改其信号处理函数为 handler，否则不做修改
 * 若 add == false，并且信号原来的处理函数为 handler，则设置信号 signum 的处理函数为 handler，否则不做修改
 */
static void uloop_install_handler(int signum, void (*handler)(int), struct sigaction* old, bool add)
{
	struct sigaction s;
	struct sigaction *act;

	act = NULL;
	sigaction(signum, NULL, &s);

	if (add) {
		if (s.sa_handler == SIG_DFL) { /* Do not override existing custom signal handlers */
			memcpy(old, &s, sizeof(struct sigaction));
			s.sa_handler = handler;
			s.sa_flags = 0;
			act = &s;
		}
	}
	else if (s.sa_handler == handler) { /* Do not restore if someone modified our handler */
			act = old;
	}

	if (act != NULL)
		sigaction(signum, act, NULL);
}

static void uloop_ignore_signal(int signum, bool ignore)
{
	struct sigaction s;
	void *new_handler = NULL;

	sigaction(signum, NULL, &s);

	if (ignore) {
		if (s.sa_handler == SIG_DFL) /* Ignore only if there isn't any custom handler */
			new_handler = SIG_IGN;
	} else {
		if (s.sa_handler == SIG_IGN) /* Restore only if noone modified our SIG_IGN */
			new_handler = SIG_DFL;
	}

	if (new_handler) {
		s.sa_handler = new_handler;
		s.sa_flags = 0;
		sigaction(signum, &s, NULL);
	}
}

/*
 * 对 SIGINT、SIGTERM、SIGCHLD 设置处理函数，并记录了信号修改前的处理状态，以便之后恢复
 */
static void uloop_setup_signals(bool add)
{
	static struct sigaction old_sigint, old_sigchld, old_sigterm;

	uloop_install_handler(SIGINT, uloop_handle_sigint, &old_sigint, add);
	uloop_install_handler(SIGTERM, uloop_handle_sigint, &old_sigterm, add);

	if (uloop_handle_sigchld)
		uloop_install_handler(SIGCHLD, uloop_sigchld, &old_sigchld, add);

	uloop_ignore_signal(SIGPIPE, add); // 忽略 SIGPIPE 信号
}

static int uloop_get_next_timeout(struct timeval *tv)
{
	struct uloop_timeout *timeout;
	int diff;

	if (list_empty(&timeouts))
		return -1;

	timeout = list_first_entry(&timeouts, struct uloop_timeout, list);
	diff = tv_diff(&timeout->time, tv);
	if (diff < 0)
		return 0;

	return diff;
}

/*
 * 遍历 timeouts 链表内节点，检测到存在当前时间点可以触发的
 * 事件则触发回调函数，并从链表中移除该事件
 */
static void uloop_process_timeouts(struct timeval *tv)
{
	struct uloop_timeout *t;

	while (!list_empty(&timeouts)) {
		t = list_first_entry(&timeouts, struct uloop_timeout, list);

		if (tv_diff(&t->time, tv) > 0)
			break;

		uloop_timeout_cancel(t);
		if (t->cb)
			t->cb(t);
	}
}

/* 清空 timeouts 链表 */
static void uloop_clear_timeouts(void)
{
	struct uloop_timeout *t, *tmp;

	list_for_each_entry_safe(t, tmp, &timeouts, list)
		uloop_timeout_cancel(t);
}

/* 清空 processes 链表 */
static void uloop_clear_processes(void)
{
	struct uloop_process *p, *tmp;

	list_for_each_entry_safe(p, tmp, &processes, list)
		uloop_process_delete(p);
}

bool uloop_cancelling(void)
{
	return uloop_run_depth > 0 && uloop_cancelled;
}

int uloop_run_timeout(int timeout)
{
	int next_time = 0;
	struct timeval tv;

	uloop_run_depth++;

	uloop_status = 0;
	uloop_cancelled = false;
	while (!uloop_cancelled)
	{
		uloop_gettime(&tv);
		uloop_process_timeouts(&tv);

        /* 若运行期间存在 SIGCHLD 事件，则 do_sigchld 设置为 true
         * 调用 uloop_handle_processes 处理
         */
		if (do_sigchld)
			uloop_handle_processes();

        /* while 循环只有在 uloop_cancelled 为 true 时，才会 break,
         * 也就是只有发送 SIGINT、SIGTERM 信号时，才会退出循环 */
		if (uloop_cancelled)
			break;

		uloop_gettime(&tv);

        /* 通过查找 timeouts 链表，计算下一个超时事件的时间间隔；
         * 若链表为空返回 -1；若存在立刻应该触发的时间返回 0 */
		next_time = uloop_get_next_timeout(&tv);
		if (timeout >= 0 && timeout < next_time)
			next_time = timeout;
		uloop_run_events(next_time);
	}

	--uloop_run_depth;

	return uloop_status;
}

/*
 * 关闭描述符 poll_fd
 * 清空 timeouts、processes 链表
 */
void uloop_done(void)
{
	uloop_setup_signals(false);

	if (poll_fd >= 0) {
		close(poll_fd);
		poll_fd = -1;
	}

	if (waker_pipe >= 0) {
		uloop_fd_delete(&waker_fd);
		close(waker_pipe);
		close(waker_fd.fd);
		waker_pipe = -1;
	}

	uloop_clear_timeouts();
	uloop_clear_processes();
}

