
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


 int main(int argc, char const *argv[])
{
	pid_t my_pid,parent_pid,child_pid;
	int status;
	my_pid = getpid();
	parent_pid = getppid();
	printf("my pid is %d\n",my_pid);
	printf("my parent pid is %d\n",parent_pid);

	if((child_pid = fork()) == -1){
		perror("fork failure");	
		exit(1);
	}

	if(child_pid == 0){
		my_pid = getpid();
		parent_pid = getppid();

		printf("Child : my  pid is %d\n" , child_pid);
		printf("Child : parent pid is %d\n",parent_pid);
		printf("I am sleeping for 3 seconds\n");
		sleep(3);
		printf("Child : i wake up\n");

		exit(1);
	}
	else {
		printf("\nParent : I created the process\n");
		printf("parent : my pid is %d\n", my_pid);
		printf("parent : my parent pid is %d \n", parent_pid);
		printf("parent : my child pid is %d\n", child_pid);
		wait(NULL);
		printf("I am alone my child die\n");
		printf("my child is dead \n");
	}
	return 0;

}