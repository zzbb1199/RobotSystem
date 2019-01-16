/**
 * 滑屏解锁主界面
 */
#include <stdio.h>
#include "lcd.h"
#include "touch.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define LINE1_END_Y 380
#define LINE2_START_Y 450
#define RECT_WIDTH 80
#define RECT_HEIGHT 60

int fd_touch = -1;

static struct Point rect  = { 0, 385 };
/* 绘制滑动引导线 */
static struct Point line1_start = { 0, LINE1_END_Y - 10 };
static struct Point line1_end = { LCD_WIDTH, LINE1_END_Y };

static struct Point line2_start = { 0, LINE2_START_Y };
static struct Point line2_end = { LCD_WIDTH, LINE2_START_Y + 10 };





int init();

int desotry();

int re_draw_rect(struct Point rect);

int main(void)
{
	init();

	/* 滑屏解锁核心程序 */
	run();

	desotry();
	return 0;
}


int init()
{
	/*  */
	lcd_open();

	/* 触摸板 */
	fd_touch = open("/dev/input/event0", O_RDONLY);

	/* 绘制背景 */
	draw_backgroud(0x000000);

	draw_line(line1_start, line1_end, 0xffffff);
	draw_line(line2_start, line2_end, 0xffffff);

	/* 初始化scroll rect */
	draw_rect(rect, RECT_WIDTH, RECT_HEIGHT, 0xfbcdef);
	return 0;
}

int desotry()
{
	lcd_close();

	return 0;
}


/**********************花屏解锁核心程序 start****************************/

int run()
{

	struct input_event event;
	int x, y, count = 0;;
	int pre_x  = -1, pre_y = -1;
	int delta_x;

	const int lock_threshold = 20;  /* 解锁threshold */
	int isscroll = 0;
	while (1)
	{
		read(fd_touch, &event, sizeof(event));
		if (EV_ABS == event.type)
		{
			if (ABS_X == event.code)
			{
				x = event.value;
				count++;
			}
			else if (ABS_Y == event.code)
			{
				y = event.value;
				count++;
			}
		}
		if (2 == count)
		{
			count = 0;
			/* 得到一组x,y */
			if (-1 == pre_x	/* 是否为第一次按压 */
				 && (y > LINE1_END_Y && y < LINE2_START_Y)  /* Y axis */
				&& (x  > rect.x && x < rect.x + RECT_WIDTH)) /* X aixs */
			{
				printf("scroll start!\n");
				pre_x = x;
				isscroll = 1;
			}
			else if (1 == isscroll)
			{
				/* rect 移动了 */
				delta_x = x - pre_x;
				rect.x += delta_x;
				pre_x = x;
				if (rect.x + RECT_WIDTH >= LCD_WIDTH)
				{
					printf("arrive unlock \n");
				}
				re_draw_rect(rect);
			}
		}

		/* 起始点检测 */
		if (EV_KEY == event.type)
		{
			if (BTN_TOUCH == event.code && 0 == event.value)
			{
				/* 检测当前是否达到滑动边界 */
				if (LCD_WIDTH - x <= lock_threshold) /**/
				{
					printf("up unlock!!!!\n");
					return;
				}
				else
				{
					/* 重新绘制 */
					rect.x = 0;
					pre_x = -1;
					re_draw_rect(rect);
					isscroll = 0;
				}
			}
		}
	}
	return 0;
}

/**********************slide解锁核心程序 end****************************/

int re_draw_rect(struct Point rect)
{
	static struct Point line1_end_left =
	{
		0, LINE1_END_Y };
	static struct Point line2_start_right =
	{
		LCD_WIDTH, LINE2_START_Y };
	draw_line(line1_end_left, line2_start_right, 0x000000);
	draw_rect(rect, 80, RECT_HEIGHT, 0xfbcdef);
}
