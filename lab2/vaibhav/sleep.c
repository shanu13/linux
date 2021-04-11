
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	unsigned int time = (unsigned int) atoi(argv[1]);

	if (sleep(time) != 0)
	{
		perror("Sleep Interrupted: ");
		exit(EXIT_FAILURE);
	}

	return 0;
}

