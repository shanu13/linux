
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>	
#include<string.h>

#define STDIN 0
#define STDOUT 1

int main(int argc, char const *argv[])
{
	int fd[2];
	printf("Pipe starts\n");



	if(pipe(fd) == -1) {
		perror("pipe fail");
		exit(1);
	}

	 printf("Fork starts \n");

		int id = fork();

		if(id ==0 ){
			printf("Child Process\n");
			close(fd[STDOUT]);
			printf("child pipe\n");
			char *x;
			scanf("%s",x);
			write(fd[STDIN],x,strlen(x)+1);
			close(fd[STDIN]);
			exit(1);

		}else{
			wait(NULL);
			char *y;
			close(fd[STDIN]);
			read(fd[STDOUT],y,100);
			printf("Child Sent to Parent value is %s\n",y);
			close(fd[STDOUT]);
		}

	
	return 0;
}