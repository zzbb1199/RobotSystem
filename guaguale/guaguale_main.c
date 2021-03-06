/**
 * 刮刮乐主程序
 */
#include <stdio.h>
#include "touch.h"
#include "lcd.h"
#include "memory_share.h"
#include "guaguale_main.h"
#include "public_resource.h"
#include "scheduler.h"

static int min(int a, int b)
{
	return a < b ? a : b;
}

static int max(int a, int b)
{
	return a > b ? a : b;
}
static int init()
{
	lcd_open();

//	touch_open();

	// 画一个背景
	draw_backgroud(0x000000);
}
static int destory()
{

	lcd_close();

//	touch_close();

	close(fd_image);
	close(fd_touch);
}



int guaguale_main()
{
	//init
	init();


	fd_touch = open("/dev/input/event0", O_RDONLY);
	if (-1 == fd_touch)
	{
		printf("open fd_touch failed\n");
	}
	else
	{
		printf("open fd_touch success\n");
	}
	//open image
	char *image_path = "./Image/image1.bmp";
	fd_image = open(image_path, O_RDONLY);
	if (-1 == fd_image)
	{
		perror("open image failed!!!");
		return -1;
	}

	//处理offset
	//lseek(fd_image,54,SEEK_SET);
	char image_info[54];
	read(fd_image, image_info, 54);

	printf("=========this image'info========\n");
	printf("image path:%s\n", image_path);
	int width = image_info[19] << 8 | image_info[18];
	int height = image_info[23] << 8 | image_info[22];
	printf("widht=%d\n", width);
	printf("height=%d\n", height);
	printf("width=%x\n", width);
	printf("height=%x\n", height);

	char buffer[width * height * 3];
	int ret = read(fd_image, buffer, width * height * 3);

	if (width > LCD_WIDTH || height > LCD_HEIGHT)
	{
		printf("this image is too large, please change a image and try again!\n'");
		return -1;
	}

	//定义图片边界
	int start_x, start_y;
	int end_x, end_y;
	start_x = LCD_WIDTH / 2 - width / 2;
	start_y = LCD_HEIGHT / 2 - height / 2;
	end_x = LCD_WIDTH / 2 + width / 2;
	end_y = LCD_HEIGHT / 2 + height / 2;

	printf("pic start %d,%d\n", start_x, start_y);
	printf("pic end %d,%d\n", end_x, end_y);

	//output
	int x, y;
	int x1, y1;
	int x2, y2;
	int color_index;
	int count = 0;

	/* delta计算辅助变量 */
	int delta_s_x;
	int delta_e_x;
	int delta_x;
	const int exit_threshold = 600;
	struct input_event event;
	while (1)
	{
		read(fd_touch, &event, sizeof(event));
		if (EV_ABS == event.type)
		{
			if (ABS_X == event.code)
			{
				x1 = event.value;
				count++;
			}
			else if (ABS_Y == event.code)
			{
				y1 = event.value;
				count++;
			}
		}
		if (2 == count)
		{
			count = 0;
			printf("guaguale touch %d,%d\n", x1, y1);
			//检测是否超过边界
			if (x1 < start_x || y1 < start_y || x1 > end_x || y1 > end_y)
			{
				continue;
			}

			//绘制点边界
			x2 = min(x1 + 20, LCD_WIDTH);
			y2 = min(y1 + 20, LCD_HEIGHT);
			x1 = max(x1 - 20, 0);
			y1 = max(y1 - 20, 0);

			//绘制点
			for (y = y1; y < y2; y++)
			{
				for (x = x1; x < x2; x++)
				{
					//printf("(%d,%d)\n",x,y);
					color_index = (LCD_HEIGHT - (y - start_y) - 1) * width + x - start_x;
					lcd_draw_point(x, y, buffer[color_index * 3 + 0] << 0 |
								   buffer[color_index * 3 + 1] << 8 |
								   buffer[color_index * 3 + 2] << 16);
				}
			}
		}

		/* 起始点检测 */
		if (EV_KEY == event.type)
		{
			if (BTN_TOUCH == event.code && 1 == event.value)
			{
				delta_s_x = x;
			}
			else if (BTN_TOUCH == event.code && 0 == event.value)
			{
				delta_e_x = x;
				delta_x =  delta_s_x  - delta_e_x;
				/* 判定是否退出 */
				if (-delta_x > exit_threshold)
				{
					printf("exit guaguale\n");
					condition = MENU;
					break;
				}
			}
		}
	}

	destory();
	return 0;
}



