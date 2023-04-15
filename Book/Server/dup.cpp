#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[]){
    if(argc <= 2){
        printf("usage : %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client_address ;
    bzero(&client_address, sizeof(client_address));
    socklen_t client_len = sizeof(client_address);
    int confd = accept(sock ,(struct sockaddr*)&client_address, &client_len);
    if(confd < 0){
        printf("errno is : %d\n", errno);
    } else {
        close(STDOUT_FILENO);
        dup(confd);
        printf("你好！！帅哥\n");
        close(confd);
    }
    close(sock);
    return 0;
}