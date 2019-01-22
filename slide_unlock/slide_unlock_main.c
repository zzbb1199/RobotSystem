/**
 * 滑屏解锁主界面
 */
#include <stdio.h>
#include "lcd.h"
#include "touch.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "slide_unlock_main.h"
#include "public_resource.h"
#include "scheduler.h"

#define LINE1_END_Y 380
#define LINE2_START_Y 450
#define RECT_WIDTH 80
#define RECT_HEIGHT 60


static struct Point rect;
/* 滑动引导线坐標 */
static struct Point line1_start;
static struct Point line1_end;

static struct Point line2_start;
static struct Point line2_end;

/* 解锁样式 */
is_progress_bar = 0;


static int run();

static int init();

static int desotry();

static int re_draw_rect(struct Point rect);

static int init_slide();


int slide_unlock_main()
{
	init();

	/* 滑屏解锁核心程序 */
	run();

	desotry();

	condition = MENU;
	printf("!!!!chagne condition =%d !!!\n", condition);
	return 0;
}


static int init()
{
	/*  */
	lcd_open();

	/* 触摸板 */
	fd_touch = open("/dev/input/event0", O_RDONLY);

//	/* 初始化滑動样式 */
//	is_progress_bar = 0;
	init_slide();
	return 0;
}


static int init_slide()
{
	/* 绘制背景 */
	//draw_backgroud(0x000000);

	rect.x = 0;
	rect.y = 385;

	line1_start.x = 0;
	line1_start.y = LINE1_END_Y - 10;

	line1_end.x = LCD_WIDTH;
	line1_end.y = LINE1_END_Y;

	line2_start.x = 0;
	line2_start.y = LINE2_START_Y;

	line2_end.x = LCD_WIDTH;
	line2_end.y = LINE2_START_Y + 10;

	draw_image("./Image/lock_screen.bmp");

	draw_line(line1_start, line1_end, 0xffffff);
	draw_line(line2_start, line2_end, 0xffffff);

	/* 初始化scroll rect */
	draw_rect(rect, RECT_WIDTH, RECT_HEIGHT, 0xfbcdef);
	return 0;
}

static int desotry()
{
	lcd_close();

	close(fd_touch);
	return 0;
}


/**********************花屏解锁核心程序 start****************************/

static int run()
{

	printf("current lock style %s\n",
		    is_progress_bar == 1 ? "progress bar" : "slide bar");
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
			if (-1 == pre_x /* 是否为第一次按压 */
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
					/**
					 * 跳转!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					 */
					printf("arrive unlock \n");
					return 0;
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
				if (LCD_WIDTH - rect.x <= lock_threshold) /**/
				{
					/**
					 * 跳转!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					 */
					printf("up unlock!!!!\n");
					return 0;
				}
				else
				{
					/* 重新绘制 */
					rect.x = 0;
					pre_x = -1;
					lcd_draw_bmp_with_start("./Image/lock_screen_slice.bmp", 0, LINE1_END_Y); 
					re_draw_rect(rect);
					isscroll = 0;
				}
			}
		}
	}
	return 0;
}

/**********************slide解锁核心程序 end****************************/

static int re_draw_rect(struct Point rect)
{
//	static struct Point line1_end_left =
//	{
//		0, LINE1_END_Y };
//	static struct Point line2_start_right =
//	{
//		LCD_WIDTH, LINE2_START_Y };
//	draw_line(line1_end_left, line2_start_right, 0x000000);
	if (!is_progress_bar)
	{
		lcd_draw_bmp_with_start("./Image/lock_screen_slice.bmp", 0, LINE1_END_Y);
	}
	draw_rect(rect, 80, RECT_HEIGHT, 0xfbcdef);
}
