#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
int main(int argc, char* argv[]){
    if(argc <= 2){
        printf("usage : %s ip_address,port_number recv_buff_size\n", basename(argv[0]));
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int recvbuf = atoi(argv[3]);
    int len = sizeof(recvbuf);

    setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, len);
    getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, (socklen_t *)&len);
    printf("the tcp receive buffer size after setting is %d\n", recvbuf);

    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in clientaddress;
    bzero(&clientaddress, sizeof(clientaddress));
    socklen_t client_length = sizeof(clientaddress);

    int confd = accept(sock, (struct sockaddr *)&clientaddress, &client_length);
    if(confd < 0){
        printf("errno is :%d \n", errno);
    } else {
        char buffer[BUFFER_SIZE];
        memset(buffer, '\0', BUFFER_SIZE);
        while(recv(confd, buffer, BUFFER_SIZE-1, 0) > 0) {}
        close(confd);
    }
    close(sock);
    return 0;

}