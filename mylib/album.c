
#include "album.h"


int init_album(char *path[], int num)
{
	if (num <= 0)
	{
		return -1;
	}
	int i;
	printf("loaded files:\n");
	for (i = 0; i < num; i++)
	{
		image_path[i] = path[i];
		printf("%s\n", image_path[i]);
	}
	image_num = num;
	draw_image(image_path[0]);
	return 0;
}

/**
 * 下一张图片
 * 
 * @author raven (19-1-13)
 * 
 * @return int 
 */
int next_image()
{
	i = (i + 1) % image_num;
	draw_image(image_path[i]);
	return 0;
}


/**
 * 上一张图片
 * 
 * @author raven (19-1-13)
 * 
 * @return int 
 */
int pre_image()
{
	i = (i - 1 + image_num) % image_num;
	draw_image( image_path[i]);
	return 0;
}
