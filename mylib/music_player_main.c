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
#include "public_resource.h"
#include "scheduler.h"

/**
 * 初始化一些参数
 * 
 * @author raven (19-1-13)
 * 
 * @return int 
 */


static struct Boundary pre_music_bd;
static struct Boundary pause_bd;
static struct Boundary next_music_bd;
static struct Boundary music_back_bd;


/**
 * 初始化边界条件
 */
static int init_boundary()
{
	pre_music_bd.p1 = (struct Point *)malloc(sizeof(struct Point));
	pre_music_bd.p2 = (struct Point *)malloc(sizeof(struct Point));
	pre_music_bd.p1->x = 163;
	pre_music_bd.p1->y = 365;
	pre_music_bd.p2->x = 198;
	pre_music_bd.p2->y = 409;

	pause_bd.p1 = (struct Point *)malloc(sizeof(struct Point));
	pause_bd.p2 = (struct Point *)malloc(sizeof(struct Point));
	pause_bd.p1->x = 220;
	pause_bd.p1->y = 365;
	pause_bd.p2->x = 268;
	pause_bd.p2->y = 409;

	next_music_bd.p1 = (struct Point *)malloc(sizeof(struct Point));
	next_music_bd.p2 = (struct Point *)malloc(sizeof(struct Point));
	next_music_bd.p1->x = 288;
	next_music_bd.p1->y = 365;
	next_music_bd.p2->x = 322;
	next_music_bd.p2->y = 409;

	music_back_bd.p1 = malloc(sizeof(struct Point));
	music_back_bd.p2 = malloc(sizeof(struct Point));
	music_back_bd.p1->x = 31;
	music_back_bd.p1->y = 35;
	music_back_bd.p2->x = 96;
	music_back_bd.p2->y = 81;
	return 0;
}


static int destory_boundary()
{
	free_boundary(&pre_music_bd);
	free_boundary(&pause_bd);
	free_boundary(&next_music_bd);
	free_boundary(&music_back_bd);
	return 0;
}

static int init()
{


	touch_open();

	//初始化音乐播放器
	init_music(music_path, music_num);

	//初始化图片封面的边界条件
	init_boundary();
	return 0;
}

static int destory()
{
	touch_close();
	lcd_close();
	destory_boundary();
	return 0;
}

/**
 * 是否点击了一首前
 * 
 * @author gec (14/01/19)
 * 
 * @return int 
 */
static int is_pre_music(int x, int y)
{
	if (x > pre_music_bd.p1->x && y > pre_music_bd.p1->y &&
		x < pre_music_bd.p2->x && y < pre_music_bd.p2->y)
	{
		return 1;
	}
	return 0;
}

static int is_pause(int x, int y)
{
	if (x > pause_bd.p1->x && y > pause_bd.p1->y &&
		x < pause_bd.p2->x && y < pause_bd.p2->y)
	{
		return 1;
	}
	return 0;
}

static int is_next_music(int x, int y)
{
	if (x > next_music_bd.p1->x && y > next_music_bd.p1->y &&
		x < next_music_bd.p2->x && y < next_music_bd.p2->y)
	{
		return 1;
	}
	return 0;
}


static int is_back(int x, int y)
{
	if (x > music_back_bd.p1->x && y > music_back_bd.p1->y &&
		x < music_back_bd.p2->x && y < music_back_bd.p2->y)
	{
		return 1;
	}
	return 0;
}

int music_player_main(int comefrom)
{
	//init
	init();
	/* 播放一首音乐 */
	start_music();
	int x, y;
	while (1)
	{
		get_xy(&x, &y);
		printf("%d,%d\n", x, y);
		if (is_pre_music(x, y))
		{
			printf("pre music\n");
			pre_music();

		}
		else if (is_pause(x, y))
		{
			pause_or_play();
		}
		else if (is_next_music(x, y))
		{
			next_music();
		}
		else if (is_back(x, y))
		{
			printf("is_back\n");
			printf("kill music\n");
			system("killall -9 madplay");
			if (VOICE_RECON == comefrom)
			{
				back2voice();
			}
			else
			{
				condition = MENU;
			}
			break;
		}
	}
	return 0;
}
