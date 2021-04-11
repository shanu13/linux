

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char *argv[])
{
	char dir[256];

	getcwd(dir, 256);

	strcat(dir, "/pwd");

	if (chdir(argv[1]) != 0)
	{
		perror("Changing Directory: ");
		exit(EXIT_FAILURE);
	}

	/*
	getcwd(dir, 256);
	printf("%s\n", dir);
	*/

	return 0;
}
