#include <sys/socket.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <fcntl.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10

int  setnonblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}
void addfd(int epollfd,int fd,bool enable_et){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if(enable_et) {
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);  //设置为非阻塞
}

void lt(epoll_event* events, int number, int epollfd, int listensock) {
    char buf[BUFFER_SIZE];
    for (int i = 0; i < number; i++) {
        int sockfd = events[i].data.fd;
        if(sockfd == listensock) {
            struct sockaddr_in client_address;
            socklen_t client_len = sizeof(client_address);
            int confd = accept(listensock, (struct sockaddr *)&client_address, &client_len);
             char buf[1024];
            memset(buf, '\0', 1024);
            inet_ntop(AF_INET, &client_address.sin_addr,buf,sizeof(buf));
            printf("IP ： %s， port ：%d 连接上服务器\n", buf, ntohs(client_address.sin_port));
            addfd(epollfd, confd, false); //对confd禁用ET模式
        } else if(events[i].events & EPOLLIN) {
            printf("event trigger once \n");
            memset(buf, '\0', BUFFER_SIZE);
            int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
            if(ret <= 0) {
                // if((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                //         printf("print later\n");
                //         continue;
                //     }
                close(sockfd);
                continue;
            }
           
            printf("get %d bytes of content: %s\n", ret, buf);
        } else {
            printf("something else happened\n");
        }
    }
}

void et(epoll_event* events, int number, int epollfd, int listensock) {
    char buf[BUFFER_SIZE];
    for (int i = 0; i < number; i++) {
        int sockfd = events[i].data.fd;
        if(sockfd == listensock) {
            struct sockaddr_in client_address;
            socklen_t client_len = sizeof(client_address);
            int confd = accept(sockfd, (struct sockaddr *)&client_address, &client_len);
            assert(confd > 0);
            addfd(epollfd, confd, true); // 开启ET模式；
        } else if(events[i].events & EPOLLIN) {
            printf("event trigger once\n");
            while(1) {
                memset(buf, '\0', BUFFER_SIZE);
                int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
                if(ret == -1 ){
                    if((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                        printf("print later\n");
                        break;
                    }
                    close(sockfd);
                    break;
                } else if(sockfd == 0) {
                    printf("lost line !!!");
                    close(sockfd);
                } else {
                    printf("get %d bytes of content : %s \n", ret, buf);
                }
            }
        } else {
            printf("something else happened!!! \n");
        }
    }
}
int main(int argc, char* argv[]) {
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
    // inet_pton(AF_INET, ip, &address.sin_addr);
    // char buf[1024];
    // memset(buf, '\0', 1024);
    // inet_ntop(AF_INET, &address.sin_addr,buf,sizeof(buf));
    address.sin_port = htons(port);

    int listensock = socket(PF_INET, SOCK_STREAM, 0);
    int ret = bind(listensock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(listensock, 5);
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5); //内核事件注册表
    assert(epollfd != -1);
    addfd(epollfd, listensock, true);

    while(1) {
        int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, 0);
        if(ret < 0) {
            printf("epoll failure\n");
            break;
        }
        lt(events, ret, epollfd, listensock);
        //lt(events, ret, epollfd, listensock);
    }
    close(listensock);
    return 0;
}