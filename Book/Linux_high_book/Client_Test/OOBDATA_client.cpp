#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if(argc <= 2){
        printf("usage : %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);    //这三行等效
    //inet_aton(ip, &server_address.sin_addr);
    //inet_pton(AF_INET, ip, &server_address.sin_addr);
   
   
    server_address.sin_port = htons(port);
    //inet_aton(argv[2], (struct in_addr* )&server_address.sin_port); // 这样不行，可以编译，但端口不正确。
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    if(connect(sockfd, (struct sockaddr* )&server_address, sizeof(server_address)) < 0){
        printf("connection failed\n");
    } else {
        const char *oob_data = "abc";
        const char *normal_data = "123";
        send(sockfd, normal_data, strlen(normal_data), 0);
        send(sockfd, oob_data, strlen(oob_data), MSG_OOB);
        send(sockfd, normal_data, strlen(normal_data), 0);
        close(sockfd);
        return 0;
    }
}