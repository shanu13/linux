

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


#define STDIN 0
#define STDOUT 1
#define SIZE 256


/* Count the total number of pile commands are present */
int pipe_count(char *cmd)
{
	int count = 0;

	for ( ; *cmd != '\0'; cmd++)
	{
		if (*cmd == '|')
			count++;
	}

	return count;
}


int is_comma_present(char *cmd)
{
	for ( ; *cmd != '\0'; cmd++)
	{
		if (*cmd == ';')
			return 1;
	}

	return 0;
}


/* Input contains the cmd as single string and size contains number of command */
char **parse_cmd(char *arg, int size)
{
	char **temp = (char **) calloc (3 * size, sizeof(char *));
	int index = 0;

	int comma_present = is_comma_present(arg);

	for (int i = 0; i < 3 * size; i++)
		temp[i] = (char *) calloc (10, sizeof(char));

	//removing newline character at the end of input
	char *input = strtok_r(arg, "\n", &arg);
	char *out;


	if (comma_present)
		out = strtok_r(input, ";", &input);
	else
		out = strtok_r(input, "|", &input);

	while (out)
	{
		char *out_word = strtok_r(out, " ", &out);

		while (out_word)
		{
			strcpy(temp[index], out_word);
			index++;

			out_word = strtok_r(out, " ", &out);
		}

		if (index % 3 != 0)
			index = ((index / 3) + 1) * 3;

		if (comma_present)
			out = strtok_r(input, ";", &input);
		else
			out = strtok_r(input, "|", &input);
	}

	return temp;

}


void execute_program(char **cmd, int is_pipe_present, int pipe_num)
{
	/*
	 * List of all programs to be executed
	 * 0 - ls
	 * 1 - pwd
	 * 2 - cd
	 * 3 - cp
	 * 4 - cat
	 * 5 - mkdir
	 * 6 - echo
	 * 7 - chdir
	 * 8 - chmod
	 * 9 - sleep
	 * 10 - tail
	 * 11 - sed
	 *
	 * */

	//if pipe is present along the command
	if (is_pipe_present)
	{
		int rename_value = 0;

		rename_value = rename("temp_input.txt", "temp.txt");
		rename_value = rename("temp_output.txt", "temp_input.txt");
		rename_value = rename("temp.txt", "temp_input.txt");

		if (rename_value != 0)
		{
			perror("Renaming temp file: ");
			exit(EXIT_FAILURE);
		}
	}

	int fd_input = 0, fd_output = 0;

	//if this command has pipe or its the last commands
	if (is_pipe_present && pipe_num)
	{
		if ((fd_input = open("temp_input.txt", O_RDONLY)) == -1)
		{
			perror("Error opening file in execute: ");
			exit(EXIT_FAILURE);
		}

		if ((fd_output = open("text_output.txt", O_WRONLY)) == -1)
		{
			perror("Error opening file in execute: ");
			exit(EXIT_FAILURE);
		}

		dup2(fd_input, STDIN);
		close(fd_input);

		dup2(fd_output, STDOUT);
		close(fd_output);
	} else if (is_pipe_present && !pipe_num)
	{
		if ((fd_input = open("text_input.txt", O_RDONLY)) == -1)
		{
			perror("Error opening file in execute: ");
			exit(EXIT_FAILURE);
		}

		dup2(fd_input, STDIN);
		close(fd_input);
	}
/*
	char *(cmd + 0)[10] = "\0";
	char *(cmd + 1)[10] = "\0";
	char *(cmd + 2)[10] = "\0";

	strcpy(*(cmd + 0), *(cmd + 0));
	strcpy(*(cmd + 1), *(cmd + 1));
	strcpy(*(cmd + 2), *(cmd + 2));
	*/

	if (strcmp(*(cmd + 0), "ls") == 0)
		execl("./ls", "./ls", *(cmd + 1), *(cmd + 2), NULL);
	else if (strcmp(*(cmd + 0), "pwd") == 0)
		execl("./pwd", "./pwd", *(cmd + 1), *(cmd + 2), NULL);
	else if (strcmp(*(cmd + 0), "chdir") == 0)
		execl("./chdir", "./chdir", *(cmd + 1), *(cmd + 2), NULL);
	else if (strcmp(*(cmd + 0), "mkdir") == 0)
		execl("./mkdir", "./mkdir", *(cmd + 1), *(cmd + 2), NULL);
	else if (strcmp(*(cmd + 0), "chmod") == 0)
		execl("./chmod", "./chmod", *(cmd + 1), *(cmd + 2), NULL);
	else if (strcmp(*(cmd + 0), "sleep") == 0)
		execl("./sleep", "./sleep", *(cmd + 1), *(cmd + 2), NULL);
	else if (strcmp(*(cmd + 0), "tail") == 0)
		execl("./tail", "./tail", *(cmd + 1), *(cmd + 2), NULL);
	else if (strcmp(*(cmd + 0), "echo") == 0)
		execl("./echo", "./echo", *(cmd + 1), *(cmd + 2), NULL);
	else
	{
		if (*(cmd + 1)[0] != '\0' && *(cmd + 2)[0] != '\0')
			execl("./cat", "./cat", *(cmd + 1), *(cmd + 2), NULL);
		else if (*(cmd + 1)[0] != '\0')
			execl("./cat", "./cat", *(cmd + 1), NULL);
	}
}


int main(int argc, char *argv[])
{
	char prompt[] = "shantanu_agrawal/CS232/shell: $ ";

	int is_pipe_present = 0, pipe_num = 0;

	while (1)
	{
		write(STDOUT, prompt, sizeof(prompt));

		char input[SIZE];

		read(STDIN, input, SIZE);

		//count the number of pipe commands present
		//if return value == 0 then no pipe is present
		pipe_num = pipe_count(input);
		is_pipe_present = pipe_num > 0 ? 1 : 0;

		//create array for pipe by size and pipe them
		int fd_pipe[2 * pipe_num];

		for (int i = 0; i < pipe_num; i++)
			pipe(fd_pipe + (2 * i));

		//parse the commands using strtok function
		int cmd_size = 3 * (pipe_num + 1);
		char **cmd;

		cmd = parse_cmd(input, pipe_num + 1);

		for (int i = 0; i < 3 * (pipe_num + 1); i++)
			printf("%s\t", cmd[i]);
		printf("\n");

		//fork the child for the Executing
		// + 1 because parent always fork for command execution
		int fork_num = pipe_num + 1;
		int fork_level[fork_num];
		int fork_index = 0;

		//if pipe is present then temporary create a file and use it for input and output and iterate along while loop
		int fdPipe_input = 0, fdPipe_output = 0;

		printf("Done creating pipes\n");

		if (pipe_num)
		{
			if ((fdPipe_input = open("temp_input.txt", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == -1)
			{
				perror("Temp Input Creation: ");
				exit(EXIT_FAILURE);
			}

			if ((fdPipe_output = open("temp_output", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == -1)
			{
				perror("Temp Output Creation: ");
				exit(EXIT_FAILURE);
			}
		}

		while (pipe_num)
		{
			fork_level[fork_num] = fork();

			if (fork_level[fork_num] > 0)
			{
				wait(NULL);
			} else
			{
				execute_program(cmd, is_pipe_present, pipe_num);
				cmd += 3;
				fork_num++;
			}
		}


		printf("Going for fork\n");

		//if only one command is provided
		fork_level[fork_index] = fork();

		if (fork_level[fork_index] > 0)
		{
			wait(NULL);
		} else if (fork_level[fork_index] == 0)
		{
			execute_program(cmd, is_pipe_present, pipe_num);
		} else
		{
			perror("Fork: ");
			exit(EXIT_FAILURE);
		}
	}


	return 0;

}
