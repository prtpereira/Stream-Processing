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

int main (int argc, char *argv[]) {

	if (argc<3) _exit(0);

	int fserver = open("fserver", O_WRONLY);

	char *linha = (char*)malloc(sizeof(char)*512);

	int n=0;
	for(int i=0; i<argc; i++) {

		n+=sprintf(linha+n, "%s ", argv[i]);
	}

	write(fserver,linha,n);

	close(fserver);
	return 0;
}
