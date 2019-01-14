
#include "dir.h"

int read_files(char *dir_path, char *type
			   , char *save_path[], int *num)
{
	printf("read files start\n");
	DIR *dir = opendir(dir_path);
	if(!dir)
	{
		perror("open dir failed");
		return -1;
	}
	struct dirent *file;
	char file_name[40];
	char *ret;
	int i = 0;
	while((file = readdir(dir)))
	{
		strcpy(file_name, file->d_name);	/*读取文件名*/
		ret = strstr(file_name, type);
		if(!ret || strlen(ret) != strlen(type))	/*如果没有该子串或者type的长度和从子串点开始的长度不同时*/
		{
			continue;
		}
		sprintf(save_path[i], "%s%s%s", dir_path, "/", file_name);
//		strcpy(save_path[i], file_name);
		i++;
	}
	*num = i;
	closedir(dir);
	printf("read files end\n");
	return 0;
}


