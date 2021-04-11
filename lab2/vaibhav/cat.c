

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define STDIN 0
#define STDOUT 1

int main(int argc, char *argv[])
{
	printf("Argc: %d\n", argc);
	printf("Argv[1] %s\n", argv[1]);
	
	//checking if both input and output files are given or not
	int input_file = 0, output_file = 0;

	if (argc == 3)
	{
		input_file = 1;
		output_file = 1;

		if (argv[1][0] == '<')
			argv[1]++;

		if (argv[2][0] == '>')
			argv[2]++;
	} else if (argc == 2)
	{
		output_file = 1;

		if (argv[1][0] == '<' || argv[1][0] == '>')
			argv[1]++;

		printf("%s\n", argv[1]);

	}

	int fd_input = 0, fd_output = 0;
	char data = '\0';

	//both input and output files are present
	if (input_file && output_file)
	{
		if ((fd_input = open(argv[1], O_RDONLY)) == -1)
		{
			perror("Opening input file: ");
			exit(EXIT_FAILURE);
		}

		if ((fd_output = open(argv[2], O_WRONLY)) == -1)
		{
			perror("Opening Output file: ");
			exit(EXIT_FAILURE);
		}

		close(STDIN);
		close(STDOUT);

		dup(fd_input);
		dup(fd_output);

		close(fd_input);
		close(fd_output);

		do
		{
			data = getchar();

			if (data != EOF)
				putchar(data);
		} while (data != EOF);
	} else if (output_file)
	{
		if (*(argv[1] - 1) == '>')
		{
			if ((fd_output = open(argv[1], O_WRONLY)) == -1)
			{
				perror("Opening Output file: ");
				exit(EXIT_FAILURE);
			}

			close(STDOUT);
			dup(fd_output);
			close(fd_output);

			do
			{
				data = getchar();

				if (data != EOF)
					putchar(data);
			} while (data != EOF);
		} else
		{
			if ((fd_output = open(argv[1], O_RDONLY)) == -1)
			{
				perror("Opening output file: ");
				exit(EXIT_FAILURE);
			}

			close(STDIN);
			dup(fd_output);
			close(fd_output);

			do
			{
				data = getchar();

				if (data != EOF)
					putchar(data);
			} while (data != EOF);
		}
	}

	return 0;
}
