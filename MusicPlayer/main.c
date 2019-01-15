/**
 * 音乐播放器主程序
 */
#include <stdio.h>
#include "touch.h"
#include "lcd_info.h"
#include "music.h"
#include "dir.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "draw.h"
#include "memory_share.h"

/**
 * 初始化一些参数
 * 
 * @author raven (19-1-13)
 * 
 * @return int 
 */


static int fd_touch = -1;
static int fd_lcd = -1;
static int *addr = NULL;

static struct Boundary pre_music_bd;
static struct Boundary pause_bd;
static struct Boundary next_music_bd;


/**
 * 初始化边界条件
 */
int init_boundary()
{
	pre_music_bd.p1 = (struct Point *)malloc(sizeof(struct Point));
	pre_music_bd.p2 = (struct Point *)malloc(sizeof(struct Point)); 
	pre_music_bd.p1->x = 130;
	pre_music_bd.p1->y = 350;
	pre_music_bd.p2->x = 170;
	pre_music_bd.p2->y = 395; 

	pause_bd.p1 = (struct Point *)malloc(sizeof(struct Point));
	pause_bd.p2= (struct Point *)malloc(sizeof(struct Point));
	pause_bd.p1->x = 200;
	pause_bd.p1->y = 350;
	pause_bd.p2->x = 256;
	pause_bd.p2->y = 395;

	next_music_bd.p1 = (struct Point *)malloc(sizeof(struct Point));
	next_music_bd.p2 = (struct Point *)malloc(sizeof(struct Point));
	next_music_bd.p1->x = 279;
	next_music_bd.p1->y = 350;
	next_music_bd.p2->x = 316;
	next_music_bd.p2->y = 395;
	return 0;
}

int init()
{
	//初始化触摸板
	fd_touch = open("/dev/input/event0", O_RDONLY);
	if (-1 == fd_touch)
	{
		perror("open touch error!");
		return -1;
	}
	fd_lcd = open("/dev/fb0", O_RDONLY);
	addr = my_mmap();

	char *music_path[50];    /*音乐文件路径*/
	int i;
	for (i = 0; i < 50;i++)
	{
		music_path[i] = (char *)malloc(sizeof(char) * 50);
	}
	int music_num = -1;
	//获取音乐文件路径
	read_files("./Music", ".mp3", music_path, &music_num);
	//初始化音乐播放器
	init_music(music_path, music_num);
	//绘制背景图
	image_show(addr, "./Image/music_pause.bmp");
	//初始化图片封面的边界条件
	init_boundary();
	return 0;
}

int destory()
{
	close(fd_lcd);
	close(fd_touch);
	my_munmap(addr);
	return 0;
}

/**
 * 是否点击了一首前
 * 
 * @author gec (14/01/19)
 * 
 * @return int 
 */
int is_pre_music(int x,int y)
{
	if (x > pre_music_bd.p1->x && y > pre_music_bd.p1->y &&
		x < pre_music_bd.p2->x && y < pre_music_bd.p2->y)
	{
		return 1;
	}
	return 0;
}

int is_pause(int x, int y)
{
	if (x > pause_bd.p1->x && y > pause_bd.p1->y &&
		x < pause_bd.p2->x && y < pause_bd.p2->y)
	{
		return 1;
	}
	return 0;
}

static is_next_music(int x, int y)
{
	if (x > next_music_bd.p1->x && y > next_music_bd.p1->y &&
		x < next_music_bd.p2->x && y < next_music_bd.p2->y)
	{
		return 1;
	}
	return 0;
}

int main(void)
{
	//init
	init();
	int isplayed = 1;
	int x, y;
	while (1)
	{
		get_xy(fd_touch, &x, &y);
		if (is_pre_music(x,y))
		{
			pre_music();
			image_show(addr, "./Image/music_pause.bmp");
		}
		else if (is_pause(x,y))
		{
			pause_or_play();
			if (isplayed)
			{
				image_show(addr, "./Image/music_play.bmp");
				isplayed = 0;
			}
			else
			{
				image_show(addr,"./Image/music_pause.bmp");
				isplayed = 1;
			}
		}
		else if (is_next_music(x,y))
		{
			next_music();
			image_show(addr, "./Image/music_pause.bmp");
		}
		/**
		 * 可加一个点击区域，推出这个循环，暂时就不管了
		 */
//		sleep(1);	/*防止无触*/
	}
	return 0;
}
