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
#define TAM PIPE_BUF


int filtra(char *buf, char *op, int x, int y) {

    //se condiçao falsa return 1 para adicionar ao output

    if (strcmp(op,">")==0) {

        if (x>y){
            return 1;
        }
        else
            return 0;
    }
    else {
        if (strcmp(op,">=")==0) {
            if (x>=y){
                return 1;
            }
            else
                return 0;
        }
        else {
            if (strcmp(op,"<")==0) {
                if (x<y){
                    return 1;
                }
                else
                    return 0;
            }
            else {
                if (strcmp(op,"<=")==0) {
                    if (x<=y){
                        return 1;
                    }
                    else
                        return 0;
                }
                else {
                    if (strcmp(op,"=")==0) {
                        if (x==y){
                            return 1;
                        }
                        else
                            return 0;
                    }
                    else {
                        if (strcmp(op,"!=")==0) {
                            if (x!=y){
                                return 1;
                            }
                            else
                                return 0;
                        }
                        else
                            return 0;
                    }
                }
            }
        }
    }


}


void main(int argc, char *argv[]) {

    if (argc<4 || argc>5)
        _exit(0);

    int valor1=atoi(argv[1]);
    char *op=argv[2]; // " "=" ", " ">=" ", " "<=" ", " "<" ", " ">" ", " "!=" "
    int valor2=atoi(argv[3]);
    size_t size = TAM;
    char *buf=(char *)malloc(sizeof(char)*size);
    char *buf3=(char *)malloc(sizeof(char)*512);
    int total=0;
    char *buf2;


    char *formato = (char*) malloc(sizeof(char)*16);     //id num
    if (argc==5)
        sprintf(formato, "id %s", argv[4]);

    //ler linha
    int n;
    while ((n=getline(&buf,&size,stdin))!=-1) {
        //buf="eventoA : NUMx : eventoB : NUMy \n"

    	if (n>0) {

            int elems[32];
            int i,j;
            j=0;
            //ler inteiros

            //le a primeira coluna
            elems[j++]=atoi(buf);
            //le as colunas seguintes
            for(i=0; i<n-1 || buf[i]!='\n'; i++) {

                if (buf[i]==':') {
                    elems[j++]=atoi(buf+i+1);
                }
            }

            if (filtra(buf,op,elems[valor1-1],elems[valor2-1])) {

                if (argc==4)
                    total+=sprintf(buf3+total,"%s",buf); //append buf no final de buf

                if (argc==5) {

                    char *buf4 = (char*) malloc(sizeof(buf));
                    strcpy(buf4,buf);
                    total+=sprintf(buf3+total, "%s %s", formato, buf4);
                }

            }
        }

        else break;
    }

    write(1,buf3,total);

    free(buf);
    free(buf3);
}
