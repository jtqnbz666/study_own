#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 1024

struct fds{
    int epollfd;
    int sockfd;
};
int setnonblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}
void addfd(int epollfd, int fd, bool oneshot) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if(oneshot) {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}
void reset_oneshot(int epollfd, int fd) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}
void * worker(void * arg) {
    int sockfd = ((fds *)arg)->sockfd;
    int epollfd = ((fds *)arg)->epollfd;
    printf("start new thread to receive data on fd : %d\n", sockfd);

    char buf[BUFFER_SIZE];
    memset(buf, '\0', BUFFER_SIZE);
    while(1) {
        int ret = recv(sockfd, buf, BUFFER_SIZE, 0);
        if(ret <= 0) {
            if(errno == EAGAIN) {
                reset_oneshot(epollfd, sockfd);
                printf("read later\n");
                break;
            }
            close(sockfd);
            break;
        } else {
            printf("get content: %s \n", buf);
            //休息五秒假装工作
           // sleep(5);
        }
    }
    printf("end thread receiving data on fd: %d\n", sockfd);
}
int main(int argc, char* argv[]) { 
    if(argc <= 2) {
        printf("usage : %s ip_addr port_number \n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    // address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(listenfd, 5);
    assert(ret != -1);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd, listenfd, false);
    while(1) {
        int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if (ret < 0) {
            printf("epoll failure\n");
            break;
        }
        for (int i = 0; i < ret; i++) {
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd) {
                struct sockaddr_in client_address;
                socklen_t client_len = sizeof(client_address);
                int confd = accept(sockfd, (struct sockaddr *)&client_address, &client_len);
                addfd(epollfd, confd, true);
            } else if (events[i].events & EPOLLIN) {
                pthread_t thread;
                fds fds_new_worker;
                fds_new_worker.epollfd = epollfd;
                fds_new_worker.sockfd = sockfd;
                pthread_create(&thread, NULL, worker, (void*)&fds_new_worker);
            } else {
                printf("something else happend\n");
            }
        }
    }
    close(listenfd);
    return 0;
}