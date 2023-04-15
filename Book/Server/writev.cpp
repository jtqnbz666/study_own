#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/uio.h>

#define BUFFER_SIZE 1024
static const char* status_line[2] = {"200 OK", "500 Internal server error"};
int main(int argc, char* argv[]){
    if(argc <= 3){
        printf("usafe : %s ip_address port_number filename\n",basename(argv[0]));
        return 0;
    }
    const char * ip = argv[1];
    int port = atoi(argv[2]);
    const char* file_name = argv[3];
    
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family =  AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    
    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);
    
    ret = listen(sock, 5);
    assert(ret != -1);
    
    struct sockaddr_in client_address;
    bzero(&client_address, sizeof(client_address));
    socklen_t client_len =  sizeof(client_address);
    
    int confd = accept(sock, (struct sockaddr*)&client_address, &client_len);
    if(confd < 0 ){
        printf("error is: %d\n", errno);
    } else {
        char head_buf[BUFFER_SIZE];
        memset(head_buf, '\0', BUFFER_SIZE);
        char* file_buf;
        struct stat file_stat;
        
        bool valid = true;

        int len = 0; //读文件的时候用。
        if(stat(file_name,&file_stat) < 0)  {  //目标文件不存在
            valid = false;
        } else {
            if(S_ISDIR(file_stat.st_mode)) {
                valid = false;
            } else if(file_stat.st_mode & S_IROTH) {
                int fd = open(file_name, O_RDONLY);
                file_buf = new char[file_stat.st_size + 1];
                memset(file_buf, '\0', file_stat.st_size + 1);
                if( read(fd, file_buf, file_stat.st_size) < 0){
                    valid = false;
                }
            }
            else {
                valid = false;
            }
        }
        if(valid) {
            ret = snprintf(head_buf, BUFFER_SIZE - 1, "%s %s\r\n", "HTTP/1.1",status_line[0]);
            len += ret;
            ret = snprintf(head_buf+len, BUFFER_SIZE - len - 1, "Content-Length: %d\r\n", file_stat.st_size);
            len += ret;
            ret = snprintf(head_buf+len, BUFFER_SIZE - len - 1, "%s", "\r\n");

            struct iovec iv[2];
            iv[0].iov_base = head_buf;
            iv[0].iov_len = strlen(head_buf);
            iv[1].iov_base = file_buf;
            iv[1].iov_len = file_stat.st_size;

            writev(confd, iv, 2);
        } else {
            ret = snprintf(head_buf, BUFFER_SIZE - 1,"%s %s\r\n", "HTTP/1.1", status_line[1]);
            len += ret;
            ret = snprintf(head_buf + len, BUFFER_SIZE -1 - len, "%s", "\r\n");
            send(confd, head_buf, strlen(head_buf), 0);
           
        }
       
         close(confd);
         delete []file_buf;
    }
        close(sock);
        return  0;
}