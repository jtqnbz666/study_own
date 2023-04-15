#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    if(argc != 2) {
        printf("usage : %s <file>\n",argv[0]);
        return 1;
    }
    int fd = open(argv[1],O_CREAT | O_WRONLY | O_TRUNC, 0666);
    assert(fd > 0);
    
    int pipefd_file[2];
    int ret = pipe(pipefd_file);
    assert(ret != -1);


    int pipefd_stdout[2];
    ret = pipe(pipefd_stdout);
    assert(ret != -1);
    
    ret = splice(STDIN_FILENO, NULL, pipefd_file[1], NULL, 32768, SPLICE_F_MOVE | SPLICE_F_MORE);
    assert(ret != -1);
    
    ret = tee(pipefd_file[0], pipefd_stdout[1], 32768, SPLICE_F_NONBLOCK);
    assert(ret != -1);

    ret = splice(pipefd_file[0], NULL,  fd, NULL, 32768, SPLICE_F_MOVE | SPLICE_F_MORE);
    assert(ret != -1);

    ret = splice(pipefd_stdout[0], NULL, STDOUT_FILENO, NULL, 32768, SPLICE_F_MOVE | SPLICE_F_MORE);
    assert(ret != -1);
    
    close(fd);
    close(pipefd_file[0]);
    close(pipefd_file[1]);
    close(pipefd_stdout[0]);
    close(pipefd_stdout[1]);
    
    return 0;
}