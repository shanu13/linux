#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

int main(int argc, char const *argv[])
{
	int id;
	int id2;

	id = fork();
	id2 = fork();

	if(id == 0){

		if(id2 == 0){
				printf("id2 =>  %d\n",getpid());
		      printf("parent id %d\n",getppid());
		      exit(1);	
		}else{
			wait(NULL);
				printf("id1 =>  %d\n",getpid());
		      printf("parent id %d\n",getppid());
		      exit(1);
		}
	}else{
		if(id2 == 0){
		    printf("id2 =>  %d\n",getpid());
		    printf("parent id %d\n",getppid());	
		    exit(1);	
		}else{
			wait(NULL);
				printf("parent =>  %d\n",getpid());
		      printf("parent id %d\n",getppid());
		}

	}

	return 0;
}