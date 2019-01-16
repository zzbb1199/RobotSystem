/**
 * 系统主界面 
 *  
 */

#include "lcd.h"
#include "touch.h"
#include <stdio.h>


static struct Boundary album_bd;
static struct Boundary video_bd;
static struct Boundary music_bd;
static struct Boundary more_bd;


/**
 * 初始化程序中所要用到的资源
 * 
 * @author gec (16/01/19)
 * 
 * @return int 
 */
int init();

/**
 * 销毁索要用到的资源
 * 
 * @author gec (16/01/19)
 * 
 * @return int 
 */
int desotry();

/**
 * 初始化菜单选项边界
 * 
 * @author gec (16/01/19)
 * 
 * @return int 
 */
int init_boundary();





/**
 * 主程序
 * 
 * @author gec (16/01/19)
 * 
 * @param void 
 * 
 * @return int 
 */
int main(void)
{
	init();
	/* 显示菜单界面 */
	draw_image("./Image/main_menu.bmp");
	/* 监控触摸event */
	int x, y;
	while (1)
	{
		get_xy(&x, &y);
		printf("%d,%d\n", x, y);
		/* 相册 */
		if (check_boundary(x, y, album_bd)) /* check函数待完善 */
		{
			printf("album\n");
		}
		/* 音乐 */
		else if (check_boundary(x, y, music_bd))
		{
			printf("music\n");
		}
		/* 视频 */
		else if (check_boundary(x, y, video_bd))
		{
			printf("video\n");
		}
		/* 更多 */
		else if (check_boundary(x, y, more_bd))
		{
			printf("more\n");
		}
	}
	desotry();
	return 0;
}




int init()
{
	/* 初始化lcd显示 */
	lcd_open();
	/* 打开触摸班 */
	touch_open();
	/* 初始化边界 */
	init_boundary();
	return 0;
}

int desotry()
{
	/* 关闭lcd */
	lcd_close();
	/* 关闭触摸班 */
	touch_close();
	return 0;
}

int init_boundary()
{
	/* 为个边界分配内存 */
	album_bd.p1 = malloc(sizeof(struct Boundary));
	album_bd.p2 = malloc(sizeof(struct Boundary));

	video_bd.p1 = malloc(sizeof(struct Boundary));
	video_bd.p2 = malloc(sizeof(struct Boundary));

	music_bd.p1 = malloc(sizeof(struct Boundary));
	music_bd.p2 = malloc(sizeof(struct Boundary));

	more_bd.p1 = malloc(sizeof(struct Boundary));
	more_bd.p2 = malloc(sizeof(struct Boundary));

	/* 初始化坐标点 */
	album_bd.p1->x = 407;
	album_bd.p1->y = 71;
	album_bd.p2->x = 493;
	album_bd.p2->y = 142;

	video_bd.p1->x = 407;
	video_bd.p1->y = 206;
	video_bd.p2->x = 493;
	video_bd.p2->y = 271;

	music_bd.p1->x = 639;
	music_bd.p1->y = 71;
	music_bd.p2->x = 701;
	music_bd.p2->y = 142;

	more_bd.p1->x = 638;
	more_bd.p1->y = 361;
	more_bd.p2->x = 707;
	more_bd.p2->y = 429;
}
