#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>	
#include <unistd.h>

#define in 0
#define out 1

int main(int argc, char const *argv[])
{

	if(argc != 3) {
		perror("invalid Arguements");
		exit(-1);
	}

	int fdinput = 0 , fdoutput = 0;

	if((fdinput = open(argv[1],O_RDONLY)) == -1){
		perror("Input file open");
		exit(-1);
	}
	if((fdoutput = creat(argv[2],0644)) == -1){
		perror("output file opening");
		exit(-1);
	}

	if(close(in) == -1){
		perror("closing stdin parent");
		exit(-1);
	}
 
 
 	if(dup(fdinput) == -1 ){
 		perror("dup parent failed");
 		exit(-1)
  	}

  	if(close(fdinput) == -1 ){
 		perror("dup parent failed");
 		exit(-1)
  	}


   if(close(out) == -1){
		perror("closing stdout parent");
		exit(-1);
	}

	if(dup(fdoutput) == -1 ){
 		perror("dup parent failed");
 		exit(-1)
  	}

  	if(close(fdoutput) == -1 ){
 		perror("dup parent failed");
 		exit(-1)
  	}

  	int fdpipe[2] = {0,0};

  	if(pipe(fdpipe) == -1){
  		perror("pipe failed");
  		exit(-1);
  	}

  	 int rfork =0, wfork=0;

  	 printf("Fork Starts");

  	 rfork = fork();

  	 if(rfork > 0){
  	 	wfork = fork();
  	 	if(wfork > 0){
  	 		close(fdpipe(out));
  	 		wait(NULL);
  	 	}else {
  	 		if (close(STDOUT) == -1)
			{
				perror("Closing_STDOUT_Child: ");
				exit(-1);
			}
			if (dup(fdPipe[out]) == -1)
			{
				perror("Pipeling_STDOUT_Child: ");
				exit(-1);
			}
			if (close(fdPipe[STDOUT]) == -1)
			{
				perror("Closing_Pipeline_STDOUT_Child: ");
				exit(-1);
			}

			execl("./c","./c",NULL)''
  	 	}

  	 }else if(rfork == 0){
  	 		close(in);
  	 		dup(fd[in]);
  	 		close(fd[in]);
  	 		close(fd[out]);
  	 		excel("./b","./b",NULL);
  	 		printf("executed read child");
  	 	}else{
  	 		perror("forking read failed");
  	 		exit (-1);
  	 	}

  	 	close(fd[in]);
  	 	close(fd[out]);


	return 0;
}