#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>
#include <errno.h>

int timeout_connect(const char * ip, int port ,int time) {
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    struct timeval timeout;
    timeout.tv_sec = time;
    timeout.tv_usec = 0;
    socklen_t len = sizeof(timeout);
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, len);
    assert(ret != -1);
    ret = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    if(ret == -1) {
        if(errno == EINPROGRESS) {
            printf("connecting timeout, process timeout logic\n");
            return -1;
        }
        printf("errno occur when connecting to server\n");
        return -1;
    }
    return sockfd;
}
int main(int argc, char* argv[]) {
    if(argc <= 2) {
        printf("usage : %s ip_addr port_number\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    int sockfd = timeout_connect(ip, port, 10);
    if(sockfd < 0) {
        return 1;
    }
    printf("连接成功！\n");
    return 0;
}