#include "theme_change_main.h"
#include "scheduler.h"
#include "lcd.h"
#include "touch.h"
#include <stdlib.h>

static struct Boundary theme1_bd;
static struct Boundary theme2_bd;

int theme_choose = THEME1;
static int init_boundary()
{
	/* 分配内存 */
	theme1_bd.p1 = malloc(sizeof(struct Boundary));
	theme1_bd.p2 = malloc(sizeof(struct Boundary));

	theme2_bd.p1 = malloc(sizeof(struct Boundary));
	theme2_bd.p2 = malloc(sizeof(struct Boundary));

	theme1_bd.p1->x = 515;
	theme1_bd.p1->y = 84;
	theme1_bd.p2->x = 618;
	theme1_bd.p2->y = 169;


	theme2_bd.p1->x = 515;
	theme2_bd.p1->y = 290;
	theme2_bd.p2->x = 620;
	theme2_bd.p2->y = 380;

	return 0;
}

static int init()
{
	theme_choose = 0;

	lcd_open();

	touch_open();

	init_boundary();

	/* 绘制bg */
	draw_image("./Image/theme_choose_background.jpg");
	return 0;
}


static int destory()
{
	lcd_close();
	touch_close();
	free_boundary(&theme1_bd);
	free_boundary(&theme2_bd);
	return 0;
}


int theme_change_main()
{
	init();

	int x,y,delta_x,delta_y;
	const int exit_threshold = 500;
	while(1)
	{
		scroll(&delta_x, &delta_y, &x, &y);
		/* 主题一 */
		if(check_boundary(x,y,theme1_bd))
		{
			printf("theme 1\n");
			theme_choose = THEME1;
			system("cp ./Theme1/* ./Image");
			sleep(2);
			break;
		}
		/* 主题二 */
		if(check_boundary(x,y,theme2_bd))
		{
			printf("theme 2\n");
			theme_choose = THEME2;
			system("cp ./Theme2/* ./Image");
			sleep(2);
			break;
		}
		if(-delta_x > exit_threshold)
		{
			break;
		}
	}
	condition = MENU2;
	destory();
	return 0;
}
