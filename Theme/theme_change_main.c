#include "theme_change_main.h"
#include "scheduler.h"
#include "lcd.h"
#include "touch.h"
#include <stdlib.h>

static struct Boundary theme1_bd;
static struct Boundary theme2_bd;


static int init_boundary()
{
	/* 分配内存 */
	theme1_bd.p1 = malloc(sizeof(struct Boundary));
	theme1_bd.p2 = malloc(sizeof(struct Boundary));

	theme2_bd.p1 = malloc(sizeof(struct Boundary));
	theme2_bd.p2 = malloc(sizeof(struct Boundary));

	if(THEME1 == theme_choose )
	{
		printf("theme1 icon coord set\n"); 
		theme1_bd.p1->x = 515;
		theme1_bd.p1->y = 84;
		theme1_bd.p2->x = 618;
		theme1_bd.p2->y = 169;


		theme2_bd.p1->x = 515;
		theme2_bd.p1->y = 290;
		theme2_bd.p2->x = 620;
		theme2_bd.p2->y = 380;
	}
	else if(THEME2 == theme_choose)
	{
		printf("theme2 icon coord set\n");
		theme1_bd.p1->x = 84;
		theme1_bd.p1->y = 122;
		theme1_bd.p2->x = 178;
		theme1_bd.p2->y = 206;


		theme2_bd.p1->x = 84;
		theme2_bd.p1->y = 306;
		theme2_bd.p2->x = 171;
		theme2_bd.p2->y = 383;
	}


	return 0;
}

static int init()
{
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
	printf("theme choose is %d\n", theme_choose);
	init();

	int x,y,delta_x,delta_y;
	const int exit_threshold = 500;
	while(1)
	{
		scroll(&delta_x, &delta_y, &x, &y);
		printf("%d,%d\n", x, y);
		/* 主题一 */
		if(check_boundary(x,y,theme1_bd))
		{
			printf("theme 1\n");
			theme_choose = THEME1;
			system("echo \"theme1\" > theme && cp ./Theme1/* ./Image");
			sleep(1);
			break;
		}
		/* 主题二 */
		if(check_boundary(x,y,theme2_bd))
		{
			printf("theme 2\n");
			theme_choose = THEME2;
			system("echo \"theme2\" > theme && cp ./Theme2/* ./Image");
			sleep(1);
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
