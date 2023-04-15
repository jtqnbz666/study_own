#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char * argv[]){
    if(argc <= 2) {
        printf("usage: %s ip_address port_numver \n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);
    
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    
    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(sock, 5);
    assert(ret != -1);
    
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    
    int confd = accept(sock, (struct sockaddr*)&client_len, &client_len);
    if(confd < 0) {
        printf("errno is: %d\n", errno);
    } else {
        int pipefd[2];
        ret = pipe(pipefd);
        assert(ret != -1);
        ret = splice(confd, 0, pipefd[1], 0, 32768, SPLICE_F_MOVE|SPLICE_F_MORE);
        assert(ret != -1);
        ret = splice(pipefd[0], 0, confd, 0, 32768, SPLICE_F_MORE|SPLICE_F_MOVE);
        assert(ret != -1);
        
        close(confd);
    }
    close(sock);
    return 0;
}