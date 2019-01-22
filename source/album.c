
#include "album.h"


int init_album()
{
	read_files("./Image", "image", image_path, &image_num);
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
	album_i = (album_i + 1) % image_num;
	draw_image(image_path[album_i]);
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
	album_i = (album_i - 1 + image_num) % image_num;
	draw_image(image_path[album_i]);
	return 0;
}
