#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MAXSIZE         1024
#define IPADDRESS       "127.0.0.1"
#define SERV_PORT       8787
#define FDSIZE          1024
#define EPOLLEVENTS     20

static void add_event(int epoll_fd, int fd, int state)
{
    struct epoll_event ev;

    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
}

static void delete_event(int epoll_fd, int fd, int state)
{
    struct epoll_event ev;

    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
}

static void do_read(int epoll_fd, int fd, int sockfd, char *buf)
{
    int read_byte;

    read_byte = read(fd, buf, MAXSIZE);
    if (read_byte == -1)
    {
        perror("read error:");
        close(fd);
    }
    else if (read_byte == 0)
    {
        fprintf(stderr, "server close.\n");
        close(fd);
    }
    else
    {
        if (fd == STDIN_FILENO)
        {
            add_event(epoll_fd, sockfd, EPOLLOUT);
        }
        else
        {
            delete_event(epoll_fd, sockfd, EPOLLIN);
            add_event(epoll_fd, STDOUT_FILENO, EPOLLOUT);
        }
    }
}

static void modify_event(int epoll_fd, int fd, int state)
{
    struct epoll_event ev;

    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev);
}

static void do_write(int epoll_fd, int fd, int sockfd, char *buf)
{
    int write_byte;

    write_byte = write(fd, buf, strlen(buf));
    if (write_byte == -1)
    {
        perror("write error:");
        close(fd);
    }
    else
    {
        if (fd == STDOUT_FILENO)
        {
            delete_event(epoll_fd, fd, EPOLLOUT);
        }
        else
        {
            modify_event(epoll_fd, fd, EPOLLIN);
        }
    }

    memset(buf, 0, MAXSIZE);
}

static void handle_events(int epoll_fd, struct epoll_event *events, int num, int sockfd, char *buf)
{
    int i;
    int fd;

    for (i = 0; i < num; i++)
    {
        fd = events[i].data.fd;
        if (events[i].events & EPOLLIN)
        {
            do_read(epoll_fd, fd, sockfd, buf);
        }
        else if (events[i].events & EPOLLOUT)
        {
            do_write(epoll_fd, fd, sockfd, buf);
        }
    }
}

static void handle_connection(int sockfd)
{
    int ret;
    int epoll_fd;
    char buf[MAXSIZE];
    struct epoll_event events[EPOLLEVENTS];

    epoll_fd = epoll_create(FDSIZE);
    add_event(epoll_fd, STDIN_FILENO, EPOLLIN);
    for (;;)
    {
        ret = epoll_wait(epoll_fd, events, EPOLLEVENTS, -1);
        handle_events(epoll_fd, events, ret, sockfd, buf);
    }

    close(epoll_fd);
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    // inet_pton(AF_INET, IPADDRESS, &server_addr.sin_addr);
    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    handle_connection(sockfd);
    close(sockfd);

    return 0;
}

