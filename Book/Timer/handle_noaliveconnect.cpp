#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#include "sort_timer_lst.h"

#define FD_LIMIT 65535
#define MAX_EVENT_NUMBER 1024
#define TIMESLOT 20
#define USER_LIMIT 5
static int pipefd[2];
static sort_timer_lst timer_lst;
static int epollfd = 0;
int setnonblock(int fd) {
    int oldoption = fcntl(fd, F_GETFL);
    int newoption = oldoption | O_NONBLOCK;
    fcntl(fd, F_SETFL, newoption);
    return oldoption;
}
void addfd(int epollfd, int fd) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblock(fd);
}

void sig_handler(int sig) {
    int save_errno = errno;
    int msg = sig;
    send(pipefd[1], &msg, 1, 0);
    errno = save_errno;
}

void addsig(int sig) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert((sigaction(sig, &sa, NULL) != -1));
}

void timer_handler() {
    //定时处理任务，实际上就是调用tick函数
    timer_lst.tick();
    //因为alarm调用只会引起一次SIGALRM信号， 所以需要重新定时，不断触发SIGALRM信号
    alarm(TIMESLOT);
}

//定时器回调函数
void cb_func(client_data* user_data) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
    assert(user_data);
    close(user_data->sockfd);
   // printf("close fd %d \n", user_data->sockfd);
    printf("客户 %s 断开网络连接！ \n", inet_ntoa(user_data->address.sin_addr));
}

int main(int argc, char* argv[]) {
    if(argc <= 2) {
        printf("usage : %s ip_addr port_number!\n", basename(argv[0]));
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
    assert(ret != -1);
    setnonblock(pipefd[1]);
    addfd(epollfd, pipefd[0]);

    //设置信号处理函数
    addsig(SIGALRM);
    addsig(SIGTERM);
    bool stop_server = false;

    client_data *users = new client_data[FD_LIMIT];
    client_data *fds = new client_data[USER_LIMIT];
    int user_count = 0;
    bool timeout = false;
    alarm(TIMESLOT); // 定时

    while(!stop_server) {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if((number < 0) && (errno != EINTR)) {
            printf("epoll failure\n");
            break;
        }
        for (int i = 0; i < number; i++) {
            int sockfd = events[i].data.fd ;
            if(sockfd == listenfd) {
                
                struct sockaddr_in client_address;
                bzero(&client_address, sizeof(client_address));
                socklen_t client_len;
                int confd = accept(listenfd, (struct sockaddr *)&client_address, &client_len);
                if(confd < 0)
                printf("eoono is : %d\n", errno);
                addfd(epollfd, confd);
                users[confd].address = client_address;
                users[confd].sockfd = confd;
                users[confd].index = user_count;
                //创建定时器
                util_timer *timer = new util_timer;
                timer->user_data = &users[confd];
                timer->cb_func = cb_func;
                time_t cur = time(NULL);
                timer->expire = cur + 3 * TIMESLOT;
                printf("加入一位客户 ip:%s , port: %d \n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                users[confd].timer = timer;
                timer_lst.add_timer(timer);
                fds[user_count].sockfd = confd;
                fds[user_count].address = client_address;
                user_count++;
                
            } else if ((sockfd == pipefd[0]) && (events[i].events & EPOLLIN)) {
                int sig;
                char signals[1024];
                ret = recv(pipefd[0], signals, sizeof(signals), 0);
                if(ret == -1) {
                    // handle the error;
                    continue;
                } else if (ret == 0) {
                    continue;
                } else {
                    for (int i = 0; i < ret; i++) {
                        switch(signals[i]) {
                            case SIGALRM: {
                                //用timeout变量标记有定时任务需要处理，但不立即处理，因为它的优先级不是很高，我们优先处理其他更重要的任务。
                                timeout = true;
                                break;
                            }
                            case SIGTERM: {
                                stop_server = true;
                            }
                        }
                        
                    }
                }
            } else if (events[i].events & EPOLLRDHUP) {//这个要放在EPOLLIN前进行判断。
                fds[users[sockfd].index] = fds[user_count-1];
                user_count--;
                cb_func(&users[sockfd]);
                timer_lst.del_timer(users[sockfd].timer);
            } else if (events[i].events & EPOLLIN) {
                memset(users[sockfd].buf, '\0', BUFFER_SIZE);
                ret = recv(sockfd, users[sockfd].buf, BUFFER_SIZE - 1, 0);
                //printf("get %d bytes of client data %s from %d\n", ret, users[sockfd].buf, sockfd);
                printf("用户 %s 发来数据： %s", inet_ntoa(users[sockfd].address.sin_addr), users[sockfd].buf);
                util_timer *timer = users[sockfd].timer;
                if(ret < 0) {
                    //如果发生读错误，则关闭连接，并移除其对应的定时器
                    if(errno != EAGAIN) {
                        cb_func(&users[sockfd]);
                        if(timer) {
                            timer_lst.del_timer(timer);
                        }
                    }
                } else if (ret == 0) {
                    //如果对方关闭了连接， 我们也关闭连接，并移除定时器
                    cb_func(&users[sockfd]);
                    if(timer) {
                        timer_lst.del_timer(timer);
                    }
                } else { //如果某个客户连接上有数据可读，则我们需要调整该连接对应的定时器，以延迟该连接被关闭的时间。
                    if(timer) {
                        time_t cur = time(NULL);
                        timer->expire = cur + 3 * TIMESLOT;
                        printf("用户 %s 的定时器刷新\n",inet_ntoa(users[sockfd].address.sin_addr));
                        timer_lst.adjust_timer(timer);
                    }
                }
                for (int i = 0; i < user_count; i++) {
                    if(fds[i].sockfd == sockfd) {
                        continue;
                    }
                    send(fds[i].sockfd, users[sockfd].buf, strlen(users[sockfd].buf), 0);
                }
            }   else {
                // others;
            }
        }
        //最后处理定时时间，因为I/O事件有更高的优先级，但这样将导致定时任务不能精确的而按照预期时间进行。
        if(timeout) {
            timer_handler();
            timeout = false;
        }
    }
    close(listenfd);
    close(pipefd[0]);
    close(pipefd[1]);
    delete[] users;
    return 0;
}