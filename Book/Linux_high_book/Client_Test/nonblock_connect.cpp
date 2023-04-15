#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/ioctl.h>

int setnonblock(int fd) {
    int oldopt = fcntl(fd, F_GETFL);
    int newopt = oldopt | O_NONBLOCK;
    fcntl(fd, F_SETFL, newopt);
    return oldopt;
}
int unblock_connect(const char* ip, int port, int time) {
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);
    // inet_pton(AF_INET, ip, &address.sin_addr);
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);
    int fdopt = setnonblock(sockfd);
    int ret = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    
    if(ret == 0) { //表示连接成功！
        printf("connect with server immediately \n");
        fcntl(sockfd, F_SETFL, fdopt);
        return sockfd;
    } else if (errno != EINPROGRESS) {
        printf("unblock connect not support!\n");
        return -1;
    }
    fd_set readfds;
    fd_set writefds;
    struct timeval timeout;
    
    FD_ZERO(&writefds);
    FD_SET(sockfd, &writefds);
    timeout.tv_sec = time;
    timeout.tv_usec = 0;

    ret = select(sockfd + 1, NULL, &writefds, NULL, &timeout);
    if(ret <= 0) {
        printf("connect time out!\n");
        close(sockfd);
        return -1;
    }
    if( ! FD_ISSET(sockfd, &writefds)) {
        printf("no events on sockfd fount!\n");
        close(sockfd);
        return -1;
    }
    int error = 0;
    socklen_t len = sizeof(error);
    if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
        printf("get sockoption failed ! \n");
        close(sockfd);
        return -1;
    }
    if(error != 0) {
        printf("connection failed after select with the errno :%d\n", errno);
        close(sockfd);
        return -1;
    }
    printf("connection ready after select with the socket :%d\n", sockfd);
    fcntl(sockfd, F_SETFL, fdopt);
    return sockfd;


}
int main(int argc, char * argv[]) {
    if(argc <= 2) {
        printf("usage : %s ip_addr port_number \n",basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int sockfd = unblock_connect(ip, port, 10);
    if(sockfd < 0) {
        return 1;
    }
    close(sockfd);
    return 0;
}