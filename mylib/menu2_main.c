
#include "lcd.h"
#include "touch.h"
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "menu_main.h"


static struct Boundary voice_recon_bd;
static struct Boundary remote_control_bd;
static struct Boundary lock_style_bd;
// static struct Boundary mus_bd;
static int init_boundary()
{
	remote_control_bd.p1 = malloc(sizeof(struct Boundary));
	remote_control_bd.p2 = malloc(sizeof(struct Boundary));

	voice_recon_bd.p1 = malloc(sizeof(struct Boundary));
	voice_recon_bd.p2 = malloc(sizeof(struct Boundary));

	lock_style_bd.p1 = malloc(sizeof(struct Boundary));
	lock_style_bd.p2 = malloc(sizeof(struct Boundary));

	remote_control_bd.p1->x = 365;
	remote_control_bd.p1->y = 96;
	remote_control_bd.p2->x = 472;
	remote_control_bd.p2->y = 192;

	voice_recon_bd.p1->x = 599;
	voice_recon_bd.p1->y = 106;
	voice_recon_bd.p2->x = 709;
	voice_recon_bd.p2->y = 199;
	
	lock_style_bd.p1->x = 374;
	lock_style_bd.p1->y = 334;
	lock_style_bd.p2->x = 459;
	lock_style_bd.p2->y = 412;


	return 0;
}

static int init()
{
	lcd_open();
	touch_open();

	draw_image("./Image/main_menu2.bmp");

	init_boundary();
	return 0;
}


static int desotry_boundary()
{
	free_boundary(&voice_recon_bd);
	free_boundary(&remote_control_bd);
	free_boundary(&lock_style_bd);
	return 0;
}

static int desotry()
{
	/* 关闭lcd */
	lcd_close();
	/* 关闭触摸班 */
	touch_close();
	/* 释放触摸边界内存 */
	desotry_boundary();
	return 0;
}

int menu2_main()
{
	init();
	int x, y;
	int delta_x, delta_y;
	const int chang_menu_threshold = 300; /* 更换men门限 */
	while (1)
	{
		scroll(&delta_x, &delta_y, &x, &y);
		printf("x %d, y%d delta_x %d delta_y %d\n", x, y, delta_x, delta_y);
		/**
		 * 滑屏处理区域
		 */
		if (-delta_x > chang_menu_threshold)
		{
			condition = MENU;
			break;

		}
		/**
		 * 点击时间判定区域
		 */
		/* 远程控制 */
		if (check_boundary(x, y, remote_control_bd)) /* check函数待完善 */
		{
			printf("remote_control\n");
			condition = REMOTE_CONTROL;
			break;
		}
		/* chat */
		else if (check_boundary(x, y, voice_recon_bd))
		{
			printf("voice_recon_bd\n");
			condition = VOICE_RECON;
			break;
		}
		/* 锁屏样式 */
		else if (check_boundary(x, y, lock_style_bd))
		{
			printf("lock_style\n");
			condition = LOCK_STYLE;
			break;
		}
	}
	desotry();
	return 0;
}
