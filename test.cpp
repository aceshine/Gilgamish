#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	char cwd[1024] = { 0 };
	char path[1024] = { 0 };
	if (getcwd(cwd, sizeof(cwd)/sizeof(char)) != NULL)
	{
		fprintf(stdout, "%s\n", cwd);
		sprintf(path, "%s/test.txt", cwd);
		fprintf(stdout, "%s\n", path);
	}
	else
		perror("getcwd error");
	return 0;
}
