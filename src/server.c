#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define NOS 100
#define TAM PIPE_BUF
#define TAM_PALAVRA 32
#define NUM_PALAVRAS 16

typedef struct node {
  int dest;
  struct node *next;
} *Tabela[NOS];


void adicionar_no(Tabela tb, int o, int d) {

    if (o==d) return;

    struct node *new;
    new = malloc(sizeof(struct node));

    new -> dest = d;
    new -> next = NULL;

    struct node *p;

    if (tb[o]==NULL) {
        tb[o]=new;
    }
    else {
        for (p = tb[o]; p; p=p->next) {
            if (p->dest == d) break;
            if (p->next == NULL) {
                p->next=new;
                break;
            }
        }
    }
}


void remover_ligacao(Tabela tb, int o, int d) {

    struct node *p;
    struct node *ant=tb[o];

    if (tb[o]==NULL) {
        return;
    }
    else {
        p=tb[o];
        if (p->dest==d){
            tb[o]=p->next;
            free(p);
            return;
        }
        else {
            for (p = tb[o]; p; p=p->next) {
                if (p->dest == d) {
                    ant->next=p->next;
                    free(p);
                    return;
                }
                ant=p;
            }
        }
    }
}



int encontrar_adjacentes(Tabela tb, int o, int *verts) {

    int res=0;

    struct node *p;

    for(p=tb[o]; p; p=p->next) {
        verts[res++]=p->dest;
    }

    return res;

}

void mostrar_tabela(Tabela tb, int n) {

    struct node *p;

    for (int i=0; i<n; i++){
        printf("[%d]: ", i);

        for(p=tb[i]; p; p=p->next) {
            printf("%d ", p->dest);
            if (p->next != NULL)
                printf("--> ");
        }

        printf("\n");
    }
}

int encontrar_anterior(Tabela tb, int n) {

    struct node *p;
    for(int o=0; o<NOS; o++) {

        for(p=tb[o]; p; p=p->next) {

            if (p->dest==n) return o;
        }
    }

    return -1;
}


int split(char *buf, char **palavra){

	int i,j;
    memset(*palavra, 0, sizeof(*palavra));

    //removes espacos iniciais
    //for (j=0; buf[j]==' '; j++);

	//for ( i = j; buf[i] != '\0' && buf[i] != ' '; i++);
	for ( i = 0; buf[i] != '\0' && buf[i] != ' '; i++);
	memcpy(*palavra,buf,i);
	//memcpy(*palavra,buf+j,i-j);

    //return i-j;
	return i;
}

ssize_t readln(int fd, char *buf, int count){

	char *aux = buf;
	int x=0;
	int flag=1;
	while(x < count && flag != 0){

		flag = read(fd,&aux[x],1);
		if(flag == -1){
			char *erro = "Erro de leitura\n";
			write(1,&erro, 17);
			exit(-1);
		}
		if(aux[x]=='\n') return x+1;
		x++;
	}
	return x;
}


int main(int argc, char *argv[]) {


    Tabela tb;
    //inicializar
    for (int i=0; i<NOS; i++)
        tb[i]=NULL;


    mkfifo("fserver", 0666);

    while(1) {

        char *linha = (char*) malloc(sizeof(char)*TAM);
        int n=0;
        int fserver = open("fserver", O_RDONLY);

        n=readln(fserver,linha,TAM);

        close(fserver);

        if (n==0) continue;


        char *cmd[NUM_PALAVRAS];
        for (int i=0; i<NUM_PALAVRAS; i++)
            cmd[i]=malloc(sizeof(char)*TAM_PALAVRA);

        int num_palavras_real=0;
        int tam=0;
        int tam2=0;
        int i;
        for(i=0; i<NUM_PALAVRAS; i++) {

            if (i>0 && linha[tam2-1]=='\0') break;
            //tam += split(linha+tam++, &argv2[i]); //tam++ devido ao espaço, "node 1 window 3" ===(split)==>> " 1 window 3" ===(tam++)==>> "1 window 3"
            tam = split(linha+tam2, &cmd[i]); //tam++ devido ao espaço, "node 1 window 3" ===(split)==>> " 1 window 3" ===(tam++)==>> "1 window 3"
            tam2+=tam+1;
        }
        num_palavras_real=i;
        //cmd[num_palavras_real-1][tam-1]='\0'; //remover o \n final
        //  ----------------------------------------


        if (strncmp(cmd[0],"disconnect",10) == 0) {

            int id1;
            int id2;
            id1=id2=0;
            if (num_palavras_real>2) { //bug se tentar igualar a 3  :(
                id1=atoi(cmd[1]);
                id2=atoi(cmd[2]);
                if (!(!id1 && strcmp(cmd[1],"0")!=0))
                    if (!(!id2 && strcmp(cmd[2],"0")!=0))
                        remover_ligacao(tb,id1,id2);
                mostrar_tabela(tb,15);
            }
        }

        if (strncmp(cmd[0],"connect",7) == 0) {

            int id1;
            int idx;
            int lig=-1;
            id1=idx=0;
            if (num_palavras_real>2) {
                id1=atoi(cmd[1]);

                for(int i=2; i<num_palavras_real; i++) {
                    idx=atoi(cmd[i]);
                    if (!(!idx && strcmp(cmd[i],"0")!=0)) {
                        adicionar_no(tb,id1,idx);
                    }
                }
                mostrar_tabela(tb,15);
            }
        }


        int adjs[16]; //numero maximo de adjacentes de cada no
        int nadjs=0;
        //escreve cmd[2:] no node cmd[1]
        if (strncmp(cmd[0],"id",2) == 0) {

            int id = atoi(cmd[1]);

            if (!(!id && strcmp(cmd[1],"0")!=0)) {
                nadjs=encontrar_adjacentes(tb,id,adjs);

                char *linha2=(char*)malloc(sizeof(char)*TAM);

                if (nadjs==0)
                    write(1, linha+4+strlen(cmd[1]), strlen(linha)-4-strlen(cmd[1]));

                else {
                    for(int i=0; i<nadjs; i++) {

                        char *fifono = (char*) malloc(sizeof(16));
                        sprintf(fifono,"fifo%d",adjs[i]);

                        int fd = open(fifono, O_RDONLY);
                        write(fd, linha+4+strlen(cmd[1]), strlen(linha)-4-strlen(cmd[1]));   //4 = 2("id")+2(espacos)
                        close(fd);
                        free(fifono);
                    }
                }
            }
        }

        if (strncmp(cmd[0],"inject",6) == 0) {

            int id = atoi(cmd[1]);
            if (!(!id && strcmp(cmd[1],"0")!=0)) {

                char *fifono = (char*) malloc(sizeof(5));
                sprintf(fifono,"fifo%d",id);

                int fd = open(fifono, O_WRONLY);

                dup2(fd, 1);

                if (!fork()) {

                    char *newArgv[num_palavras_real-1];
                    int i;
                    for( i = 2; i < num_palavras_real; i ++){

                        int n = strlen(cmd[i]);
                        newArgv[i] = (char*)malloc(sizeof(char)*n);
                        memcpy(newArgv[i],argv[i],n);
                    }


                    newArgv[i]=(char*)malloc(sizeof(char));
                    newArgv[i]=(char*)NULL;

                    int error = execv(newArgv[0],newArgv);
                    //printf("Não foi possivel injetar output do comando! [Inject_Error Code:Comando não encontrado]\n");
                    _exit(0);

                }
                wait(0); //esperar que o filho executa o comando
                close(fd);

                free(fifono);

            }
        }

        if (strncmp(cmd[0],"remove",6) == 0) {

            int id = atoi(cmd[1]);
            if (!(!id && strcmp(cmd[1],"0")!=0)) {

                nadjs=encontrar_adjacentes(tb,id,adjs);

                int ant = encontrar_anterior(tb,id);
                if (ant==-1) break;

                for(int i=0; i<nadjs; i++) {

                    remover_ligacao(tb, id, adjs[i]);
                    adicionar_no(tb, ant, adjs[i]);
                }

            }
        }

        free(linha);
        for (int i=0; i<NUM_PALAVRAS; i++)
            free(cmd[i]);

    }//end while


	return 0;
}
