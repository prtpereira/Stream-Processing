#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

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
#define TAM PIPE_BUFs

void main(int argc, char *argv[]) {

    if (argc<2 || argc>3)
        _exit(0);

    char *valor=argv[1];

    char *buf;
    char *buf2;
    size_t size = TAM;
    buf = (char *)malloc(size * sizeof(char));
    char *buf3=(char*)malloc(512);
    int total=0;
    //ler linha
    size_t n;


    char *formato = (char*) malloc(sizeof(char)*16);     //id num
    if (argc==3)
        sprintf(formato, "id %s", argv[2]);

    //while (n=read(0,buf,sizeof(buf))) {
    while ((n=getline(&buf,&size,stdin))!=-1) {
        //buf="eventoA : NUMx : eventoB : NUMy \n"
    	if (n>0) {

            //write(1,buf,16);
            int elems[3];
            int i,j;

            //ler os dois inteiros
            for(i=0,j=0; i<n-1 || buf[i]!='\n'; i++) {

                if (buf[i]==':'){
                    elems[j++]=atoi(buf+i+1);
                }
            }

            //scanf(buf,"%s:%d:%s:%d",elems[0],elems[1]);

            //alocar sempre espaço para buf2, de modo que possivel conteudo antigo seja descartado
            buf2=(char*)malloc(sizeof(char)*TAM);

            memcpy(buf2,buf,n-1); //copiar buf (exceto \n) para buf2

            if (argc==2)
                sprintf(buf2, "%s:%s\n", buf2,valor);

            if (argc==3) {
                char *buf4 = (char*) malloc(sizeof(buf2));
                strcpy(buf4,buf2);
                sprintf(buf2, "%s %s\n", formato, buf4);
            }

            if (elems[2]>=elems[0]) {

                total+=sprintf(buf3+total,"%s",buf2); //append buf no final de buf3
            }

            //free(buf2); //bug (NO IDEAD WHY XD) se libertar espaço
        }

        else break;

    }

    write(1,buf3,total);

    free(buf);
    free(buf2);
    free(buf3);

}
