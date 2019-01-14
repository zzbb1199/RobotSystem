
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


/**
 * 初始化一些参数
 * 
 * @author raven (19-1-13)
 * 
 * @return int 
 */


static int fd_touch = -1;

int init()
{
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
	//初始化触摸板
	fd_touch = open("/dev/input/event0", O_RDONLY);
	if (-1 == fd_touch)
	{
		perror("open touch error!");
		return -1;
	}
	return 0;
}


int main(void)
{
	//init
	init();

	int x, y;
	while (1)
	{
		get_xy(fd_touch, &x, &y);
		if (x < 300)
		{
			pre_music();
		}
		else if (x < 500)
		{
			pause_or_play();
		}
		else
		{
			next_music();
		}
		/**
		 * 可加一个点击区域，推出这个循环，暂时就不管了
		 */
		sleep(1);	/*防止无触*/
	}
	return 0;
}
