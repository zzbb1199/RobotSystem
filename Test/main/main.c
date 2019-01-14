#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int main(void)
{
	DIR *dir = opendir("./DIR");
	if(!dir)
	{
		perror("open failed");
		return -1;
	}

	struct dirent *file;
	while((file = readdir(dir)))
	{
		printf("%s\n", file->d_name);
	}
	return 0;
}
