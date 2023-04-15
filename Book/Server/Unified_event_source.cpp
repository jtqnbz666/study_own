#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <signal.h>
 static int pipefd[2];
#define MAX_EVENT_NUMBER 1024
int setnonblock(int fd) {
    int oldopt = fcntl(fd, F_GETFL);
    int newopt = oldopt | O_NONBLOCK;
    fcntl(fd, F_SETFL, newopt);
    return oldopt;
}
void addfd(int epollfd, int fd) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN |EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblock(fd);
}
// 信号处理函数
void sig_handler(int sig) {
    //保留原来的errno， 在函数最后恢复，以保证函数的可重入性
    int save_errno = errno;
    int msg = sig;
    send(pipefd[1], (char *)&msg, 1, 0);
    errno = save_errno;
}
//设置信号的处理函数
void addsig(int sig) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;//重启因为信号中断的系统调用
    sigfillset(&sa.sa_mask);
    sigdelset(&sa.sa_mask, SIGQUIT);
    sigprocmask(SIG_UNBLOCK, &sa.sa_mask, NULL);
    assert(sigaction(sig, &sa, NULL) != -1);
}
int main(int argc, char * argv[]) {
    if(argc <= 2) {
        printf("usage : %s ip_addr port_number\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

   
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);
    int ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(listenfd, 5);
    assert(ret != -1);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd, listenfd);
    ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
    setnonblock(pipefd[1]);
    addfd(epollfd, pipefd[0]);

    //设置一些信号处理函数
    addsig(SIGHUP);
    addsig(SIGCHLD);
    addsig(SIGTERM);
    addsig(SIGINT);
    bool stop_server = false;
    while(!stop_server) {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if((number < 0) && (errno != EINTR)) {
            printf("epoll failure\n");
            break;
        }
        for (int i = 0; i < number; i++) {
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd) {
                struct sockaddr_in client_address;
                socklen_t client_len = sizeof(client_address);
                int confd = accept(listenfd, (struct sockaddr *)&client_address, &client_len);
                addfd(epollfd, confd);
            } else if ((sockfd == pipefd[0]) && (events[i].events & EPOLLIN)) {
                int sig;
                char signals[1024];
                memset(signals, '\0', 1024);
                ret = recv(sockfd, signals, sizeof(signals), 0);
                if(ret == -1) {
                    continue;
                } else if (ret == 0) {
                    continue;
                } else {
                    for (int i = 0; i < ret; i++) {
                        switch(signals[i]) {
                            case SIGCHLD:
                            case SIGHUP: {}
                            case SIGTERM:
                            case SIGINT: {
                                stop_server = true;
                            }
                        }
                    }
                }
            } else {

            }
        }
    }
    printf("close fds \n");
    close(listenfd);
    close(pipefd[1]);
    close(pipefd[0]);
    return 0;
}