
/**
 * 视频播放器主程序
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "touch.h"
#include "lcd_info.h"
#include "dir.h"
#include "video.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include "public_resource.h"
#include "scheduler.h"

/**
 * 初始化一些参数
 *
 * @author raven (19-1-13)
 *
 * @return int
 */


static int fifo = -1;    /*fifo管道文件描述符*/

#define PRO_THRESHOLD  40	/*快进判定门限*/

#define VOL_THRESHOLD 5	/*音量变化门限*/

#define EXIT_THRESEHOLD 600

#define DOUBLE_THRESHOLD 300 /*双击检测门限: 第一次和第二次点击之间的最长时间差*/
/**
 * x的绝对值
 *
 * @author gec (14/01/19)
 *
 * @param x
 *
 * @return inollt
 */
int abs(int x)
{
	return x > 0 ? x : -x;
}

#define SCROLL_X 0x01
#define SCROLL_Y 0x02
#define SCROLL_EXIT 0x03


/**
 * 滑动处理
 *
 * @author gec (14/01/19)
 *
 * @param delta
 * @param flag
 *
 * @return int
 */
static int scroll_tackle(int delta, int flag)
{
	switch (flag)
	{
	case SCROLL_X:
		if (abs(delta) > PRO_THRESHOLD)
		{
			if (delta > 0) /*后退条件*/
			{
				backward(-delta / 15);
			}
			else
			{
				forward(-delta / 15);
			}
		}
		break;
	case SCROLL_Y:
		if (abs(delta) > VOL_THRESHOLD)
		{
			if (delta > 0)
			{
				volume_up((float)delta);
			}
			else
			{
				volume_down((float)delta);
			}
		}
		break;
	default:
		break;
	}
	return 0;
}

/**
 * 设置x或y
 *
 * @author gec (14/01/19)
 *
 * @param x
 * @param y
 * @param count
 */
static void set_xy(int *x, int *y, int *count, struct input_event event)
{
	if (ABS_X == event.code)
	{
		*x = event.value;       //检查是否快进
		(*count)++;
	}
	else if (ABS_Y == event.code)
	{
		*y = event.value;
		(*count)++;
	}
}
static int init()
{
	//初始化触摸板
	fd_touch = open("/dev/input/event0", O_RDONLY);
	if (-1 == fd_touch)
	{
		perror("open touch error!");
		return -1;
	}
	//初始画管道控制文件
	if (access("/tmp/fifo", F_OK))
	{
		if (-1 == mkfifo("/tmp/fifo", 0666))
		{
			perror("make fifo error!");
			return -1;
		}
	}
	//初始化管道文件描述符
	fifo = open("/tmp/fifo", O_RDWR);
	if (-1 == fifo)
	{
		perror("open fifo error");
		return -1;
	}

	//初始化视频播放器
	init_video(fifo);

	return 0;
}

/**
 * 双击处理函数
 *
 * @author gec (14/01/19)
 *
 * @param delta_time 时间差
 * @param x
 *
 * @return int
 */
static int double_click_tackle(int delta_time, int x)
{

	printf("delta time %d\n", delta_time);
	if (delta_time < DOUBLE_THRESHOLD)
	{
		printf("double click!!!\n");
		//检测当前x,y区域
		if (x > 600)
		{
			next_video();
		}
		else if (x < 300)
		{
			pre_video();
		}
	}

}

/**
 * 暂停或者播放
 *
 * @author gec (14/01/19)
 *
 * @param x 当前触摸点的横坐标
 *
 * @return int
 */
static int pause_play(int x)
{
	if (x > 300 && x < 500)
	{
		play_or_pause();
	}
	return 0;
}



static int run()
{

	//参数初始化
	int x, y;
	int start_x, start_y, end_x, end_y;
	int delta_x, delta_y;
	int count = 0;
	int is_scroll = 0;      /*当前是否还在滑动*/

	struct input_event event;

	//双击检测
	struct timeval tv;
	int click_times = 0;
	//初始化点击时间
	gettimeofday(&tv, NULL);
	long pre_time = ((tv.tv_sec % 1000) * 1000) + (tv.tv_usec / 1000);;
	long current_time = pre_time;

	while (1)
	{
		read(fd_touch, &event, sizeof(event));
		//检测一： 当前触摸点设置
		if (EV_ABS == event.type)
		{
			set_xy(&x, &y, &count, event); /*该条件应该被执行两次*/
		}

		//下面括号中的代码实现，暂停和双击检测
		if (2 == count)
		{
			count = 0;  /*必须在这里更新count*/
			if (!is_scroll) /*如果当前还在滑动的话，一定不是上述两个功能*/
			{
				//功能一：暂停或者播放
				count = 0;
				printf("%d,%d\n", x, y);
				pause_play(x);

				//功能二： 双击检测
				gettimeofday(&tv, NULL);
				current_time = ((tv.tv_sec % 1000) * 1000) + (tv.tv_usec / 1000);
				printf("current time %ld\n", current_time);
				double_click_tackle(current_time - pre_time, x);
				pre_time = current_time;
			}
		}

		//检测二：按压点和起来点设置
		if (EV_KEY == event.type)
		{
			if (BTN_TOUCH == event.code && 1 == event.value)
			{
				start_x = x;
				start_y = y;
				is_scroll = 1;
			}
			else if (BTN_TOUCH == event.code && 0 == event.value)
			{
				end_x = x;
				end_y = y;
				is_scroll = 0;
				//一次滑动完成
				delta_x = start_x - end_x;
				delta_y = start_y - end_y;
				printf("delta x=%d,delta_y = %d\n", delta_x, delta_y);
				scroll_tackle(delta_x, SCROLL_X);
				scroll_tackle(delta_y, SCROLL_Y);
				//退出视频播放
				if (abs(delta_x) > EXIT_THRESEHOLD)
				{
					printf("delta_x %d thresehold %d\n", delta_x, EXIT_THRESEHOLD);
					printf("exit!!!!\n");
					break;
				}
			}
		}


	}
	return 0;
}

static int destory()
{
	system("killall -9 mplayer");
	close(fd_touch);
	close(fifo);
}

int video_out_destory()
{
	printf("video out desotry!\n");
	destory();
	return 0;
}

int video_player_main(int comefrom)
{
	//init
	init();
	/* play the first video */
	start_video();
	//run
	run();

	destory();
	if (VOICE_RECON == comefrom)
	{
		back2voice();
	}
	else
	{
		condition = MENU;
	}
	return 0;
}

