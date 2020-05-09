#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>

#define IP_ADDR         "127.0.0.1"
#define PORT            8080
#define MAX_SIZE        1024
#define LISTEN_QUEUE    128
#define FD_SIZE         1000
#define EPOLL_EVENTS    1024

/* create socket and bind */
static int socket_bind(const char *ip, int port)
{
    int opt = 1;
    int listen_fd;
    struct sockaddr_in server_addr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)
    {
        perror("socket error:");
        exit(1);
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    // inet_pton(AF_INET, ip, &server_addr.sin_addr);
    server_addr.sin_port = htons(port);
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind error: ");
        exit(1);
    }

    return listen_fd;
}

static void add_event(int epoll_fd, int fd, int operation)
{
    struct epoll_event ev;

    ev.events = operation;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
}

static void handle_accpet(int epoll_fd, int listen_fd)
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd == -1)
    {
        perror("accpet error:");
    }
    else
    {
        printf("accept a new client: %s:%d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
        add_event(epoll_fd, client_fd, EPOLLIN);
    }
}

static void delete_event(int epoll_fd, int fd, int operation)
{
    struct epoll_event ev;

    ev.events = operation;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
}

static void modify_event(int epoll_fd, int fd, int operation)
{
    struct epoll_event ev;

    ev.events = operation;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev);
}

static void do_read(int epoll_fd, int fd, char *buf)
{
    int read_byte;

    read_byte = read(fd, buf, MAX_SIZE);
    if (read_byte == -1)
    {
        perror("read error:");
        close(fd);
        delete_event(epoll_fd, fd, EPOLLIN);
    }
    else if (read_byte == 0)
    {
        fprintf(stderr, "client close.\n");
        close(fd);
        delete_event(epoll_fd, fd, EPOLLIN);
    }
    else
    {
        printf("read message is: %s", buf);
        modify_event(epoll_fd, fd, EPOLLOUT);
    }
}

static void do_write(int epoll_fd, int fd, char *buf)
{
    int write_byte;

    write_byte = write(fd, buf, strlen(buf));
    if (write_byte == -1)
    {
        perror("write error:");
        close(fd);
        delete_event(epoll_fd, fd, EPOLLOUT);
    }
    else
    {
        modify_event(epoll_fd, fd, EPOLLIN);
    }

    memset(buf, 0, MAX_SIZE);
}

static void handle_events(int epoll_fd, struct epoll_event *events, int num, int listen_fd, char *buf)
{
    int i;
    int fd;

    for (i = 0; i < num; i++)
    {
        fd = events[i].data.fd;
        if ((fd == listen_fd) && (events[i].events & EPOLLIN))
        {
            handle_accpet(epoll_fd, listen_fd);
        }
        else if (events[i].events & EPOLLIN)
        {
            do_read(epoll_fd, fd, buf);
        }
        else if (events[i].events & EPOLLOUT)
        {
            do_write(epoll_fd, fd, buf);
        }
    }
}

/* io multiplexing with epoll */
static void do_epoll(int listen_fd)
{
    int ret;
    int epoll_fd;
    char buf[MAX_SIZE];
    struct epoll_event events[EPOLL_EVENTS];

    memset(buf, 0, MAX_SIZE);
    /* create a file descripter */
    epoll_fd = epoll_create(FD_SIZE);
    /* add monitor file descripter event */
    add_event(epoll_fd, listen_fd, EPOLLIN);
    for (;;)
    {
        /* wait for the ready file descripter and handle it */
        ret = epoll_wait(epoll_fd, events, EPOLL_EVENTS, -1);
        handle_events(epoll_fd, events, ret, listen_fd, buf);
    }

    close(epoll_fd);
}

int main(int argc, char *argv[])
{
    int listen_fd;

    listen_fd = socket_bind(IP_ADDR, PORT);
    listen(listen_fd, LISTEN_QUEUE);
    do_epoll(listen_fd);

    return 0;
}

