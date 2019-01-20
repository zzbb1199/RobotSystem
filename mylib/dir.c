
#include "dir.h"
#include <stdlib.h>
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
		printf("%s\n", file->d_name);
		strcpy(file_name, file->d_name);	/*读取文件名*/
		if(0 == strcmp(type,"mp3"))
		{
			ret = strstr(file_name, ".mp3"); 
			if(!ret   || strlen(ret) != strlen(".mp3")) /*如果没有该子串或者type的长度和从子串点开始的长度不同时*/
			{
				continue;
			}
		}
		else if(0 == strcmp(type, "image"))
		{
			if(!strstr(file_name, ".bmp") && !strstr(file_name,".jpg"))
			{
				continue;
			}
		}
		else if(0 == strcmp(type, "video"))
		{
			if(!strstr(file_name,".avi"))
			{
				continue;
			}
		}
		save_path[i] = malloc(50 * sizeof(char));
		sprintf(save_path[i], "%s%s%s", dir_path, "/", file_name);
//		strcpy(save_path[i], file_name);
		i++;
	}
	*num = i;
	closedir(dir);
	printf("read files end\n");
	return 0;
}


