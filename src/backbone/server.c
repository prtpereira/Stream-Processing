#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define TAM 512

// read do 0
// escrever no 1

int main(int argc, char *argv[]) {

    int fd[2];
    pipe(fd);

    int fds = open("fildes", O_WRONLY | O_APPEND);

    char endereco[4];
    memset(endereco, 0, sizeof(endereco));

    int num = sprintf(endereco, "%d%s", fd[1],"\n");

    write(fds, endereco, num);

    char buf[TAM];

    while (1) {

        int n = read(fd[0], buf, sizeof(buf));

        if (n<=0) break;

        write(1,buf,n);
    }

    close(fd[0]);
    close(fd[1]);

    close(fds);

    return 0;
}
