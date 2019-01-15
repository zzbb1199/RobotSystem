/**
 * 登录界面主程序
 */

#include <stdio.h>
#include "touch.h"
#include "draw.h"
#include "memory_share.h"


int *addr = NULL;
int fd_image = -1;
int fd_lcd = -1;
int fd_touch = -1;

int min(int a, int b)
{
	return a < b ? a : b;
}

int max(int a, int b)
{
	return a > b ? a : b;
}
int init()
{
	addr = my_mmap();
	fd_lcd = open("/dev/fb0", O_RDWR);
	fd_touch = open("/dev/input/event0", O_RDONLY);
	if (-1 == fd_lcd || -1 == fd_touch)
	{
		perror("init failed");
		return -1;
	}
	//显示九宫格
	image_show(addr, "./Image/login.bmp");
}
int destory()
{
	my_munmap(addr);
	close(fd_touch);
	close(fd_lcd);
	close(fd_image);
}



int main(void)
{
	//init
	init();

	destory();
}





