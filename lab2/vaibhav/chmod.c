

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	//char path[256];
	uid_t uid = 0;

	uid = strtol(argv[1], NULL, 8);

	if (chmod(argv[2], uid) == -1)
	{
		perror("fchown: ");
		exit(EXIT_FAILURE);
	}

	return 0;
}

