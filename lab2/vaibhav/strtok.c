

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{

	char cmd[30][10] = {'\0', '\0'};

	char str[] = "Hello There | There is ; a bootle | on ; the table Bye";
	char *output = str;

	char *out = strtok_r(output, ";|", &output);
 
	int index = 0;

	while (out != NULL)
	{
		printf("Out: %s\n", out);

		char del[] = " ";

		char *out_word = strtok_r(out, del, &out);

		while (out_word != 0)
		{
			//cmd[index] = (char *)calloc(10, sizeof(char));
			//cmd[index] = out_word;
			
			strcpy(cmd[index], out_word);
			index++;

	//		printf("OUTWORD: %s\ncmd: %s\n", out_word, cmd[index]);

			out_word = strtok_r(out, del, &out);
		}

		//checking for index position to next line
		if (index % 3 != 0)
		{
			index = ((index / 3) + 1) * 3;
		}

		out = strtok_r(output, ";|", &output);
		printf("==============================\n");
	}
	

	printf("Printing cmd stored values\n");

	for (int i = 0; i < 30; i++)
	{
		if (i%3 == 0 && cmd[i][0] == '\0')
			break;

		if (cmd[i] != "\0")
			printf("%s\t", cmd[i]);

		if ((i + 1) % 3 == 0)
			printf("\n");


	}

	return 0;
}

