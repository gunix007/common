#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/resource.h>

#define MAX_EPOLL       1
#define MAX_LINE        1024
#define PORT            8080
#define MAX_BACK        1000

int main(int argc, char **argv)
{
    int i;
    int listen_fd;
    int conn_fd;
    int epoll_fd;
    int read_bytes;
    int cur_fds;        /* the current opened file descriptors */
    int wait_fds;       /* return value of epoll_wait */
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    struct epoll_event ev;
    struct epoll_event evs[MAX_EPOLL];
    struct rlimit rlt;  /* need for setting connect numbers */
    char buf[MAX_LINE] = {0};
    socklen_t len = sizeof(struct sockaddr_in);

    /* the max number of file descriptors for each process */
    rlt.rlim_max = rlt.rlim_cur = MAX_EPOLL;
    if (setrlimit(RLIMIT_NOFILE, &rlt) == -1)
    {
        printf("Setrlimit Error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == listen_fd)
    {
        printf("Socket Error: %d\n" , errno);
        exit(EXIT_FAILURE);
    }

    if (fcntl(listen_fd, F_SETFL, fcntl(listen_fd, F_GETFD, 0) | O_NONBLOCK) == -1)
    {
        printf("Set blocking error: %d\n", errno);
		exit(EXIT_FAILURE);
    }

    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		printf("Bind Error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

    if (listen(listen_fd, MAX_BACK) == -1)
    {
        printf("Listen Error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    epoll_fd = epoll_create(MAX_EPOLL);
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listen_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0)
    {
        printf("Epoll Error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    cur_fds = 1;

    while (1)
    {
        if ((wait_fds = epoll_wait(epoll_fd, evs, cur_fds, -1)) == -1)
        {
            printf("Epoll Wait Error: %d\n", errno);
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < wait_fds; i++)
        {
            if (evs[i].data.fd == listen_fd && cur_fds < MAX_EPOLL)
            {
                if ((conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &len)) == -1)
                {
                    printf("Accept Error: %d\n", errno);
                    exit(EXIT_FAILURE);
                }

                printf("Server get from client %s:%d!\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);

                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev) < 0)
                {
                    printf("Epoll Error: %d\n", errno);
                    exit(EXIT_FAILURE);
                }
                ++cur_fds;
                continue;
            }

			/* handle data */
            read_bytes = read(evs[i].data.fd, buf, sizeof(buf));
            if (read_bytes <= 0)
            {
                close(evs[i].data.fd);
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
                --cur_fds;
                continue;
            }

            printf("get message: %s\n", buf);
            write(evs[i].data.fd, buf, read_bytes);
            memset(buf, 0, sizeof(buf));
        }
    }

    close(listen_fd);
	return 0;
}

