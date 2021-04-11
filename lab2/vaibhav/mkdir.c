

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


int main(int argc, char *argv[])
{
	int dir = 0;

	if ((dir = mkdir(argv[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == -1)
	{
		perror("Creating directory: ");
		exit(EXIT_FAILURE);
	}

	return 0;
}

