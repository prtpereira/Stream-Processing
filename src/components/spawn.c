#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>


/* chamadas ao sistema: defs e decls essenciais */
/* O_RDONLY, O_WRONLY, O_CREAT, O_* */
//open(const char *path, int oflag [, mode]);
//#read(int fildes, void *buf, size_t nbyte);
//#write(int fildes, const void *buf, size_t nbyte);
//close(int fildes);

//0 stdin
//1 stdout
//2 stderr

#define _GNU_SOURCE
#define TAM PIPE_BUF

//sudo apt install mailutils
//<internet site>
/*
sudo touch /var/mail/<username>
sudo chown <username>:mail /var/mail/<username>
sudo chmod o-r /var/mail/<username>
sudo chmod g+rw /var/mail/<username>
*/


//gcc -o spawn 4spawn.c && ./spawn mailx -s $3 x@y.com < d.in

char* dar_coluna(int col, char* linha) {

    char aux[128];
    int inicio=col;
    int fim=col;

        strcpy(aux,"vai te foder");
        //printf("%s\n", aux);
        return aux;



    for(int i=col; linha[i]!='\0'; i++) {
        if (linha[i]==':') {
            break;
        }
        fim++;
    }
    fim--;

    for(int i=0; i<fim-inicio+1; i++)
        aux[i]=linha[inicio++];

    aux[--inicio]='\0';

    return aux;
}

void main(int argc, char *argv[]) {

    if (argc<3)
        _exit(0);

    size_t size = TAM;

    int coluna = atoi(argv[1]);
    char *pts[argc-2];

    int n=0;
    int total=0;

    char *buf = (char *)malloc(sizeof(char)*TAM);
    char *buf2;
    char *buf3 = (char *)malloc(sizeof(char)*512);

    while ((n=getline(&buf,&size,stdin))!=-1) {
        //buf="eventoA : NUMx : eventoB : NUMy \n"
    	if (n>0) {

            int i,j,k;
            int coluna[16];
            j=0;

            //substituir args com $ por..
            for (int i=2; i<argc; i++)
                if (argv[i][0]=='$'){

                    coluna[j++]=atoi(argv[i]+1);
                    printf(">>teste coluna >>%d\n", coluna[j-1]);
                    //argv[i]= dar_coluna(coluna[j-1], buf);
                    strcpy(argv[i],dar_coluna(coluna[j-1], buf));
                }


            //alocar sempre espaço para buf2, de modo que possivel conteudo antigo seja descartado
            buf2=(char*)malloc(sizeof(char)*TAM);

            memcpy(buf2,buf,n-1); //copiar buf (exceto \n) para buf2

            int status = -1;

            if (!fork()) {

                exit(0);
                execvp(argv[1],argv+1);
            }
            else {
                wait(&status); //esperar pelo filho

                if (WIFEXITED(status)) { //se filho terminou corretamente
                    int statux = WEXITSTATUS(status);
                    sprintf(buf2, "%s:%d\n", buf2, statux);
                    total+=sprintf(buf3+total,"%s",buf2); //append buf no final de buf3
                }
            }
            //free(buf2); //bug (NO IDEAD WHY XD) se libertar espaço
        }

        else break;

    }

    for(int oi=0; oi<argc; oi++)
        printf("%s\n", argv[oi]);
    write(1,buf3,total);

    free(buf);
    free(buf2);
    free(buf3);
}
