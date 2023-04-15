#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <assert.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[]){
    if(argc <= 3){
        printf("usage :  %s ip_address port_number filename\n", basename(argv[0]));
        return 1;
    }
    const char * ip = argv[1];
    int port = atoi(argv[2]);
    const char * file_name = argv[3];
    
    int fd = open(file_name, O_RDONLY);
    assert(fd > 0);
    struct stat stat_buf;
    stat(file_name, &stat_buf);
   
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);
    
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    
    int ret = bind(sock, (struct sockaddr* )&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);
    
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int confd = accept(sock, (struct sockaddr*)&client_address, &client_len);
    if(confd < 0) {
        printf(" error is : %d\n", errno);
    } else{
       
        sendfile(confd, fd, NULL, stat_buf.st_size); //NULL 为偏移量
        close(confd);
    }
    close(sock);
    return 0;


}