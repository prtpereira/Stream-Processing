#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include "biblioteca.c"

//recebe o ID do no
//recebe o comando que ele tem que executar



int main(int argc, char *argv[])
{
	if( argc < 3)
		return -1;

	int ID = atoi(argv[1]);

	//faz o seu fifo de input( Para onde o servidor vai escrever)
	char *nomeFifo = (char*) malloc( sizeof(char)* 10);
	sprintf( nomeFifo, "fifo%d", ID);
	mkfifo( nomeFifo, 0666);


	//buffer para guardar cada linha ( ./nao e preciso)
	char *linha = (char*) malloc(sizeof(char)*TAM);
	int n=0;

	while(1){

		//abre o fifo
		int fifoIn = open( nomeFifo, O_RDONLY);

		//cria pipe para enviar input para o exec
		int fd[2];
		pipe(fd);

		//le do fifo e escreve no pipe
		n = read(fifoIn, linha, TAM);

		write( fd[1], linha, n);
		close(fd[1]);
		//abre fserver
		int saida = open("fserver", O_WRONLY);

		if( !fork()){
			//redireciona

			dup2(fd[0],0);
			dup2(saida,1);


			//adiciona ao argv o NULL para o exec
			char * meuID = (char*) malloc(sizeof(char)*3);
			sprintf(meuID,"%d",ID);

			char *newArgv[argc +2];
			int i;
			for( i = 0; i < argc; i ++) {

				int n = strlen(argv[i]);
				newArgv[i] = malloc(sizeof(char)*n);
				memcpy(newArgv[i],argv[i],n);
			}

			newArgv[i]=(char*)malloc(sizeof(char));
			newArgv[i]=(char*)NULL;
			free(meuID);

			//faz exec
			int error = execv(newArgv[2],newArgv +2); //executa comando
            printf("Não foi possível executar o comando pretendido. [Node_Error Code:%d] \n",error);
			_exit(0);
		}
		wait(0); //tirar isto depois.....................TESTE

		close(saida);
		close(fd[0]);
	    n = 0;
    }

	free( nomeFifo);

	return 0;
}
