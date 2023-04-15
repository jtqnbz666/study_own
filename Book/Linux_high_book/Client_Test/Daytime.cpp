#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <arpa/inet.h>

int main(int argc, const char* argv[]){
    assert(argc == 2);
   
    
    //const char *host = argv[1];
    //struct hostent *hostinfo = gethostbyname(host);
    //printf("host: %s\n", hostinfo->h_name);
    
    const char *ip = argv[1];
    struct hostent *hostinfo = gethostbyaddr(ip, sizeof(ip), AF_INET);
     printf("host: %s\n", hostinfo->h_name);

    assert(hostinfo);
    struct servent *servinfo = getservbyname("daytime", "tcp");
    assert(servinfo);
    printf("daytime port is %d\n", ntohs(servinfo->s_port));
    printf("addr:%s ,port: %d\n",inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list),ntohs(servinfo->s_port));
    printf("hha1 ");
    struct sockaddr_in address;
    int nlen = sizeof(address);
    printf("%d", nlen);
    address.sin_family = AF_INET;
     printf("hha1 ");
    address.sin_addr = *(struct in_addr *)*hostinfo->h_addr_list;
     printf("hha2 ");
    address.sin_port = servinfo->s_port;
  printf("hha ");
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  
    printf("addr:%s ,port: %d",inet_ntoa(address.sin_addr),ntohs(address.sin_port));
    int result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    assert(result != -1);
    printf("hha ");
    char buffer[128];
    result = read(sockfd, buffer, sizeof(buffer));
    assert(result > 0);
    buffer[result] = '\0';
    printf("the day time is : %s ", buffer);
    close(sockfd);
    return 0;
}