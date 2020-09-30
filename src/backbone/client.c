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

int main(int argc, char *argv[]) {

    if (argc<1) _exit(0);

    int fd = open("backbone", O_WRONLYz);

    //dup2(soo,0);
    //dup2(soo,1);
    //close(fd);
    //close(1);
    //char *buf = (char*) malloc(TAM*sizeof(char));
    char buf[TAM];

    while(1) {

        int n = read(0,buf,sizeof(buf));

        if (n<=0) break;

        write(fd,buf,n);

    }

    close(fd);


    return 0;
}
