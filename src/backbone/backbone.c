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

void main(int argc, char *argv[]) {

	mkfifo("backbone",0666);

    int fds = open("backbone", O_RDONLY);


    int fildes = open("fildes", O_WRONLY | O_CREAT, 0666);

    size_t size = 512;
    int m=0;
    char buf[TAM];
    char *buf2 = (char*) malloc(sizeof(char)*TAM);


    //escrever em varios nos
    while (1) {

        int n = read(fds,buf,sizeof(buf));

        if (n<0) break;

        write(1,buf,n);

        printf("endereco %s\n", buf);

        while ((m=getline(&buf2, &size, stdin))!=-1) {

            if (m<0) break;

            write(atoi(buf2),buf,n);

        }

    }


}
