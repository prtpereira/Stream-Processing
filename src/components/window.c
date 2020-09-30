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


int maxi(int l, int nums[],int x) {

    int mx=0;
    //inicializar mn
    for(int i=x-1-l; i<x-1; i++)
        if (i>=0){
            mx=nums[i];
            break;
        }

    for(int i=x-1-l; i<x-1; i++)
        if (i>=0)
            if (nums[i]>mx)
                mx=nums[i];

    return mx;
}

int mini(int l, int nums[], int x) {

    int mn=0;
    //inicializar mn
    for(int i=x-1-l; i<x-1; i++)
        if (i>=0){
            mn=nums[i];
            break;
        }

    for(int i=x-1-l; i<x-1; i++)
        if (i>=0)
            if (nums[i]<mn)
                mn=nums[i];

    return mn;
}

int avg(int l, int nums[], int x) {

    int val=0;
    int d=0;
    int i;
    for(i=x-1-l; i<x-1; i++)
        if (i>=0) {
            val+=nums[i];
            d++;
        }

    //se nao leu nehuma linha
    if (i-1<0) d=1;
    return val/d;
}

int sum2(int l, int nums[], int x) {

    int sum=0;
    for(int i=x-1-l; i<x-1; i++)
        if (i>=0)
            sum+=nums[i];

    return sum;
}

int calcular(int l, char *op, int nums[], int len) {

    //char *commands[] = {"avg","max","min","sum"};
    int num=0;
    if (strcmp(op,"avg")==0) {
        num=avg(l,nums,len);
    }
    else {

        if (strcmp(op,"sum")==0) {
            num=sum2(l,nums,len);
        }
        else {

            if (strcmp(op,"min")==0) {
                num=mini(l,nums,len);
            }
            else {

                if (strcmp(op,"max")==0) {
                    num=maxi(l,nums,len);
                }
            }
        }
    }

    return num;
}






void main(int argc, char *argv[]) {

    if (argc<4 || argc>5)
        _exit(0);

    int coluna = atoi(argv[1]);
    int linha = atoi(argv[3]);
    char *comando = argv[2];
    int n=0;
    int total=0;
    size_t size = TAM;
    int x=0;
    char *buf = (char *)malloc(sizeof(char)*TAM);
    char *buf2;
    char *buf3 = (char *)malloc(sizeof(char)*512);


    char *formato = (char*) malloc(sizeof(char)*16);     //id num
    if (argc==5)
        sprintf(formato, "id %s", argv[4]);


    while ((n=getline(&buf,&size,stdin))!=-1) {
        //buf="eventoA : NUMx : eventoB : NUMy \n"
    	if (n>0) {

            //write(1,buf,16);
            int elems[32];
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
            int nums[16];

            nums[x++]=elems[coluna-2];
            memcpy(buf2,buf,n-1); //copiar buf (exceto \n) para buf2

            int out = calcular(linha,comando,nums,x);
            sprintf(buf2, "%s:%d\n", buf2,out);

            if (argc==4)
                total+=sprintf(buf3+total,"%s",buf2); //append buf no final de buf3

            if (argc==5) {

                char *buf4 = (char*) malloc(sizeof(buf2));
                strcpy(buf4,buf);
                total+=sprintf(buf3+total, "%s %s", formato, buf4);
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
