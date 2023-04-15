#define _GNU_SOURCE 1
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <poll.h>
#include <errno.h>
#include <fcntl.h>


#define USER_LIMIT 5
#define BUFFER_SIZE 64  //读缓冲区的大小
#define FD_LIMIT 65535  //文件描述符数量限制
    
struct client_data {
    struct sockaddr_in address;
    char *write_buf;
    char buf[BUFFER_SIZE];
};

int setnonblock(int fd) {
    int oldopt = fcntl(fd, F_GETFL);
    int newopt = oldopt | O_NONBLOCK;
    fcntl(fd, F_SETFL, newopt);
    return oldopt;
}
int main(int argc, char * argv[]) {
    if (argc <= 2) {
        printf("usage : %s ip_addr port_number \n",basename(argv[0]));
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

    client_data *users = new client_data[FD_LIMIT];

    pollfd fds[USER_LIMIT + 1]; //因为有个fds[0];
    int user_counter = 0;
    for (int i = 0; i <= USER_LIMIT; i++) {
        fds[i].fd = -1;
        fds[i].events = 0;
    }
    fds[0].fd = listenfd;
    fds[0].events = POLLIN | POLLERR;
    fds[0].revents = 0;
    while(1) {
        ret = poll(fds, user_counter + 1, -1);
        if(ret < 0) {
            printf("poll failure \n");
            break;
        }
        for (int i = 0; i < user_counter + 1; i++) {
            if((fds[i].fd == listenfd) && (fds[i].revents & POLLIN)) {
                struct sockaddr_in client_address;
                socklen_t client_len = sizeof(client_address);
                int confd = accept(listenfd, (struct sockaddr *)&client_address, &client_len);
                if(confd < 0) {
                    printf("errno is : %d\n", errno);
                    continue;
                } 
                if (user_counter >= USER_LIMIT) {
                    const char *info = "too many users\n";
                    printf("%s", info);
                    send(confd, info, strlen(info), 0);
                    close(confd);
                    continue;
                }
                user_counter++;
                users[confd].address = client_address;
                //setnonblock(confd);
                fds[user_counter].fd = confd;
                fds[user_counter].events = POLLIN | POLLRDHUP | POLLERR;
                fds[user_counter].revents = 0;
                printf("comes a new user, now have %d users\n", user_counter);
            } else if (fds[i].revents & POLLERR) {
                printf("get an error from %d\n", fds[i].fd);
                char errors[100];
                memset(errors, '\0', 100);
                socklen_t length = sizeof(errors);
                if(getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, errors, &length) < 0) {
                    printf("get socket option failed\n");
                }
                continue;
            } else if (fds[i].revents & POLLRDHUP) {
                users[fds[i].fd] = users[fds[user_counter].fd];
                close(fds[i].fd);
                fds[i] = fds[user_counter];
                i--;
                user_counter--;
                printf("a client left\n");
            } else if (fds[i].revents & POLLIN) {
                int confd = fds[i].fd;
                memset(users[confd].buf, '\0', BUFFER_SIZE);
                ret = recv(confd, users[confd].buf, BUFFER_SIZE - 1, 0);
                users[confd].buf[ret-1] = '\0';
                printf("get %d bytes of client data : %s from %d\n", ret, users[confd].buf, confd);
                if(ret < 0) {
                    if(errno != EAGAIN) {
                        close(confd);
                        users[fds[i].fd] = users[fds[user_counter].fd];
                        fds[i] = fds[user_counter];
                        i--;
                        user_counter--;
                    }
                } else if(ret == 0) {

                } else {
                    for (int j = 1; j <= user_counter; j++) {
                        if(fds[j].fd == confd) {
                            continue;
                        }
                        fds[j].events |= ~POLLIN;
                        fds[j].events |= POLLOUT;
                        users[fds[j].fd].write_buf = users[confd].buf;
                    }
                }
            } else if (fds[i].revents & POLLOUT) {
                int confd = fds[i].fd;
                if(!users[confd].write_buf) {
                    continue;
                }
                ret = send(confd, users[confd].write_buf, strlen(users[confd].write_buf), 0);
                users[confd].write_buf = NULL;

                fds[i].events |= ~POLLOUT;
                fds[i].events |= POLLIN;
            }
        }
    }
    delete[] users;
    close(listenfd);
    return 0;
}
