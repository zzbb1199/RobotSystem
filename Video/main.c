
/**
 * 视频播放器主程序
 */
#include <stdio.h>
#include "touch.h"
#include "lcd_info.h"
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
	char *video_path[50];    /*视频文件路径*/
	int i;
	for (i = 0; i < 50; i++)
	{
		video_path[i] = (char *)malloc(sizeof(char) * 50);
	}
	int video_num = -1;
	//获取视频文件路径
	read_files("./Music", ".avi", video_path, &video_num);
	//初始化视频播放器
	init_video(video_path, video_num);
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
		/**
		 * 触摸板检测，待写
		 */
		read(fd_touch, &event, sizeof(event));
		if(EV_ABS == event.type)
		{
			if(ABS_X == event.code)
			{
				*x = event.value;
				count++;
			}
			else if(ABS_Y == event.code)
			{
				*y = event.value;
				count++;
			}
		}
		if(count == 2)
		{
			return;
		}
	}
	return 0;
}
