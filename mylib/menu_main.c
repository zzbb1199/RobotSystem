/**
 * 系统主界面 
 *  
 */

#include "lcd.h"
#include "touch.h"
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

static struct Boundary camera_bd;
static struct Boundary video_bd;
static struct Boundary music_bd;
static struct Boundary locking_bd;
static struct Boundary guaguale_bd;
static struct Boundary chat_bd;


/**
 * 初始化程序中所要用到的资源
 * 
 * @author gec (16/01/19)
 * 
 * @return int 
 */
static int init();

/**
 * 销毁索要用到的资源
 * 
 * @author gec (16/01/19)
 * 
 * @return int 
 */
static int desotry();

/**
 * 初始化菜单选项边界
 * 
 * @author gec (16/01/19)
 * 
 * @return int 
 */
static int init_boundary();





/**
 * 主程序
 * 
 * @author gec (16/01/19)
 * 
 * @param void 
 * 
 * @return int 
 */
int menu_main()
{
	init();

	/* 监控触摸event */
	int x, y;
	int delta_x = 0; 
	int delta_y = 0;
	const int chang_menu_threshold = 300; /* 更换men门限 */
	while (1)
	{
		scroll(&delta_x, &delta_y, &x, &y);
		printf("x %d, y%d delta_x %d delta_y %d\n", x, y,delta_x,delta_y);
		/**
		 * 滑屏处理区域
		 */
		if (delta_x > chang_menu_threshold)
		{
			condition = MENU2;
			break;

		}
		/**
		 * 点击时间判定区域
		 */
		/* 相册 */
		if (check_boundary(x, y, camera_bd)) /* check函数待完善 */
		{
			printf("camera\n");
			condition = CAMERA;
			break;
		}
		/* 音乐 */
		else if (check_boundary(x, y, music_bd))
		{
			printf("music\n");
			condition = MUSIC_PLAYER;
			break;
		}
		/* 视频 */
		else if (check_boundary(x, y, video_bd))
		{
			printf("video\n");
			condition = VIDEO_PLAYER;
			break;
		}
		/* 更多 */
		else if (check_boundary(x, y, chat_bd))
		{
			printf("chat_bd\n");
			condition = CHAT;
//			break;
		}
		/* 锁屏 */
		else if (check_boundary(x, y, locking_bd))
		{
			printf("locd\n");
			condition = LOCK;
			break;
		}
		/* 刮刮乐 */
		else if (check_boundary(x, y, guaguale_bd))
		{
			printf("guagua\n");
			condition = GUAGUA;
			break;
		}


	
	}
	desotry();
	return 0;
}



static int init()
{
	/* 初始化lcd显示 */
	lcd_open();
	/* 打开触摸班 */
	touch_open();
	/* 显示菜单界面 */
	draw_image("./Image/main_menu.bmp");
	/* 初始化边界 */
	init_boundary();
	return 0;
}

static int desotry()
{
	/* 关闭lcd */
	lcd_close();
	/* 关闭触摸班 */
	touch_close();
	return 0;
}

static int init_boundary()
{
	/* 为个边界分配内存 */
	camera_bd.p1 = malloc(sizeof(struct Boundary));
	camera_bd.p2 = malloc(sizeof(struct Boundary));

	video_bd.p1 = malloc(sizeof(struct Boundary));
	video_bd.p2 = malloc(sizeof(struct Boundary));

	music_bd.p1 = malloc(sizeof(struct Boundary));
	music_bd.p2 = malloc(sizeof(struct Boundary));

	chat_bd.p1 = malloc(sizeof(struct Boundary));
	chat_bd.p2 = malloc(sizeof(struct Boundary));

	locking_bd.p1 = malloc(sizeof(struct Boundary));
	locking_bd.p2 = malloc(sizeof(struct Boundary)); 

	guaguale_bd.p1 = malloc(sizeof(struct Boundary));
	guaguale_bd.p2 = malloc(sizeof(struct Boundary));

	/* 初始化坐标点 */
	camera_bd.p1->x = 407;
	camera_bd.p1->y = 71;
	camera_bd.p2->x = 493;
	camera_bd.p2->y = 142;

	video_bd.p1->x = 407;
	video_bd.p1->y = 206;
	video_bd.p2->x = 493;
	video_bd.p2->y = 271;

	music_bd.p1->x = 639;
	music_bd.p1->y = 71;
	music_bd.p2->x = 701;
	music_bd.p2->y = 142;

	chat_bd.p1->x = 638;
	chat_bd.p1->y = 361;
	chat_bd.p2->x = 707;
	chat_bd.p2->y = 429;

	locking_bd.p1->x = 626;
	locking_bd.p1->y = 206;
	locking_bd.p2->x = 690;
	locking_bd.p2->y = 271;

	guaguale_bd.p1->x = 407;
	guaguale_bd.p1->y = 378;
	guaguale_bd.p2->x = 493;
	guaguale_bd.p2->y = 430;
	
	return 0;
}
