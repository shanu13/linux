

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char dir[256];

	getcwd(dir, 256);

	printf("%s\n", dir);

	return 0;
}

