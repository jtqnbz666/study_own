#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>
static int confd;
#define BUFFER_SIZE 1024
void sig_urg(int sig) {
    int save_errno = errno;
    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', BUFFER_SIZE);
    int ret = recv(confd, buffer, BUFFER_SIZE - 1, MSG_OOB);
    printf("got %d bytes of oob data '%s'\n", ret, buffer);
    errno = save_errno;
}
void addsig(int sig, void (*sig_handler)(int)) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

int main(int argc, char * argv[]) {
    if(argc <= 2) {
        printf("usage : %s ip_addr port_number \n", basename(argv[0]));
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

    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    confd = accept(listenfd, (struct sockaddr *)&client_address, &client_len);
    if(confd < 0) {
        printf("errno is : %d", errno);
    } else {
        addsig(SIGURG, sig_urg);
        //使用SIGURG信号之前，我们必须设置socket的宿主进程或进程组
        fcntl(confd, F_SETOWN, getpid());

        char buffer[BUFFER_SIZE];
        while(1) {
            memset(buffer, '\0', BUFFER_SIZE);
            ret = recv(confd, buffer, BUFFER_SIZE - 1, 0);
            if(ret <= 0) {
                break;
            }
            printf("got %d bytes of normal data '%s'\n", ret, buffer);
        }
        close(confd);
    }
    close(listenfd);
    return 0;
}