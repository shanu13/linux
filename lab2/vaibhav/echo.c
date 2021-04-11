

#include <stdio.h>

int main(int argc, char *argv[])
{
	char opening_bracket = '\0';

	for (int i = 1; i < argc; i++)
	{
		if (argv[1][0] == '\'' || argv[1][0] == '"')
			opening_bracket = (argv[1][0] == '"') ? '"' : '\'';

		for (int j = 0; argv[i][j] != '\0'; j++)
		{
			if (argv[i][j] != opening_bracket)
				putchar(argv[i][j]);
		}

		putchar(' ');

	}

	putchar('\n');
		
	return 0;
}
