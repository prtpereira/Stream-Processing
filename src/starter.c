#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define TAM PIPE_BUF
#define TAM_PALAVRA 32
#define NUM_PALAVRAS 16



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


int main(int argc, char *argv[]) {

    size_t size = TAM;


	 //arrancar servidor central
	if (!fork()) {
		execl("server","server",(char*)NULL);
	}

	else {

		size_t n = 0;

	    while(1) {

 			char *linha = (char*) malloc(sizeof(char)*TAM);
	        n=getline(&linha, &size, stdin);
			//n = read(1,linha,TAM);

	        if (n==-1) continue;


	                //fazer split da linha
	        //  ----------------------------------------

	        char *argv2[NUM_PALAVRAS];
	        for (int i=0; i<NUM_PALAVRAS; i++)
	            argv2[i]=malloc(sizeof(char)*TAM_PALAVRA);

			int num_palavras_real=0;
			int tam=0;
			int tam2=0;
			for(int i=0; i<NUM_PALAVRAS; i++) {

				if (i>0 && linha[tam2-1]=='\0') break;
				//tam += split(linha+tam++, &argv2[i]); //tam++ devido ao espaço, "node 1 window 3" ===(split)==>> " 1 window 3" ===(tam++)==>> "1 window 3"
				tam = split(linha+tam2, &argv2[i]); //tam++ devido ao espaço, "node 1 window 3" ===(split)==>> " 1 window 3" ===(tam++)==>> "1 window 3"
				tam2+=tam+1;
				num_palavras_real++;
			}
			argv2[num_palavras_real-1][tam-1]='\0'; //remover o \n final
	        //  ----------------------------------------



	        if (!fork()) {

				if ( strcmp(argv2[0],"inject") == 0     ||
			         strcmp(argv2[0],"connect") == 0    ||
				     strcmp(argv2[0],"disconnect") == 0 ||
				     strcmp(argv2[0],"disconnect") == 0 ) {

					int fserver=open("fserver",O_WRONLY);
					write(fserver, linha, sizeof(char)*strlen(linha)-1);
					close(fserver);
					_exit(0);
				}

				else {

					char *argv3[num_palavras_real+1];
					int i;
					for (i=0 ; i<num_palavras_real; i++) {

						int tamx = strlen(argv2[i]);
						argv3[i]=(char*)malloc(sizeof(char)*tamx);
						memcpy(argv3[i],argv2[i],tamx);

					}
					//termninador da string de palavras para ser usado no exec
					argv3[i]=(char*)malloc(sizeof(char));
					argv3[i]=(char*)NULL;

		            int error = execv(argv3[0],argv3); //executa comando
		            printf("Não foi possível executar o comando pretendido. [Controller_Error Code:%d] \n",error);
					_exit(0); //para nao correr outro processo duplicado
				}

	        }

	        //wait(0); //esperar que o filho acabe para enviar novos comandos

			free(linha);
		} //end while
	}

    return 0;
}
