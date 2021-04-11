

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int all = 0;
	DIR *pDir;

	if (argc == 3)
	{
		pDir = opendir(argv[2]);

		all = 1;

	} else if (argc == 2)
	{
		if (argv[1] == "-a")
		{
			all = 1;
			argv[2] = ".";

			pDir = opendir(argv[2]);
		} else
		{
			pDir = opendir(argv[1]);

		}
	} else if (argc == 1)
	{
		all = 0;

		pDir = opendir(".");
	}

	if (pDir == NULL)
	{
		perror("Opening the directory ");
		exit(EXIT_FAILURE);
	}

	struct dirent *opened_dir;

	while ((opened_dir = readdir(pDir)) != NULL)
	{
		//set errno equal to zero
		errno = 0;

		if (!all && opened_dir->d_name[0] == '.')
			continue;


		printf("%s\n", opened_dir->d_name);
	}

	if (errno != 0)
	{
		perror("Error during ls: ");
		exit(EXIT_FAILURE);
	}

	return 0;
}

