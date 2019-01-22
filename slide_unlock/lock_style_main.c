#include "touch.h"
#include "lcd.h"
#include "scheduler.h"
#include "lock_style_main.h"
#include <stdlib.h>

static struct Boundary progress_bar;
static struct Boundary slide_bar;
static int init_boundary()
{
	progress_bar.p1 = malloc(sizeof(struct Boundary));
	progress_bar.p2 = malloc(sizeof(struct Boundary));

	slide_bar.p1 = malloc(sizeof(struct Boundary));
	slide_bar.p2 = malloc(sizeof(struct Boundary));



	slide_bar.p1->x = 629;
	slide_bar.p1->y = 944;
	slide_bar.p2->x = 727;
	slide_bar.p2->y = 183;

	progress_bar.p1->x = 628;
	progress_bar.p1->y = 308;
	progress_bar.p2->x = 734;
	progress_bar.p2->y = 397;


	return 0;
}


static int destory_boundary()
{
	free_boundary(&progress_bar);
	free_boundary(&slide_bar);
	return 0;
}

static int init()
{
	lcd_open();

	touch_open();

	init_boundary();

	/* 绘制选择样式背景图 */
	draw_image("./Image/unlock_style_background.bmp");
	return 0;
}

static int destory()
{
	lcd_close();

	touch_close();

	destory_boundary();
	return 0;
}

int lock_style_main()
{

	init();
	int x,y,delta_x,delta_y;
	const int exit_threshold = 500;
	while(1)
	{
		scroll(&delta_x, &delta_y,& x, &y);
		printf("lock style touch :%d,%d\n", x, y);
		/**
		 * 判定滑动是否超过退出门限
		 */
		if(-delta_x > exit_threshold)
		{
			break;
		}
		/* 判定选择解锁方案 */
		if(check_boundary(x, y, progress_bar))
		{
			printf("choose progress bar\n");
			is_progress_bar = 1;
		}
		else if(check_boundary(x, y, slide_bar))
		{
			printf("choose slide bar\n");
			is_progress_bar = 0;
		}
	}

	destory();
	condition = MENU2;
	return 0;
}
