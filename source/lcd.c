/****************************************************************************************
 *文件名称:lcd.c
 *设    计:温子祺
 *日    期:2015-5-29
 *说	明:	1)支持显示任意大小的位图
			2)自动识别位图格式	
  -------------------------------------------------------------------------------------
 *修改日期:2015-6-5
			1)添加了jpg文件、jpg流显示的功能
 
 
	------------------------------------------------------------------------------------
	优化刷新图片算法:
	修改日期:2019-1-15
	设计:Raven
****************************************************************************************/
#include <stdio.h>   	//printf scanf
#include <fcntl.h>		//open write read lseek close
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"
#include "touch.h"
#include "jpeglib.h"
#include "api_v4l2.h"

static char g_color_buf[FB_SIZE] = { 0 };

static int  g_fb_fd;
static int *g_pfb_memory;

/* video_chat.c 画中画显示的坐标 */
volatile int g_jpg_in_jpg_x;
volatile int g_jpg_in_jpg_y;



static int min(int a, int b)
{
	return a < b ? a : b;
}

//初始化LCD
int lcd_open(void)
{
	g_fb_fd = open("/dev/fb0", O_RDWR);

	if (g_fb_fd < 0)
	{
		printf("open lcd error\n");
		return -1;
	}

	g_pfb_memory  = (int *)mmap(NULL,                   //映射区的开始地址，设置为NULL时表示由系统决定映射区的起始地址
								FB_SIZE,                //映射区的长度
								PROT_READ | PROT_WRITE,   //内容可以被读取和写入
								MAP_SHARED,             //共享内存
								g_fb_fd,                //有效的文件描述词
								0                       //被映射对象内容的起点
							   );

	return g_fb_fd;

}

//LCD画点
void lcd_draw_point(unsigned int x, unsigned int y, unsigned int color)
{
	*(g_pfb_memory + y * 800 + x) = color;
}

#if EN_LCD_SHOW_JPG
int lcd_draw_jpg(unsigned int x, unsigned int y, const char *pjpg_path, 
				 char *pjpg_buf, unsigned int jpg_buf_size, unsigned int jpg_half)
{
//	printf("x=%d\n", x);
	/*定义解码对象，错误处理对象*/
	struct 	jpeg_decompress_struct 	cinfo;
	struct 	jpeg_error_mgr 			jerr;

	char 	*pcolor_buf = g_color_buf;
	char 	*pjpg;

	unsigned int 	i = 0;
	unsigned int	color = 0;
	unsigned int	count = 0;

	unsigned int 	x_s = x;
	unsigned int 	x_e;
	unsigned int 	y_e;

	unsigned int 	save_x = x;
	unsigned int 	save_y = y;

	int	jpg_fd;
	unsigned int 	jpg_size;

	unsigned int 	jpg_width;
	unsigned int 	jpg_height;



	if (pjpg_path != NULL)
	{
		/* 申请jpg资源，权限可读可写 */
		jpg_fd = open(pjpg_path, O_RDWR);

		if (jpg_fd == -1)
		{
			printf("open %s error\n", pjpg_path);

			return -1;
		}

		/* 获取jpg文件的大小 */
		jpg_size = file_size_get(pjpg_path);

		/* 为jpg文件申请内存空间 */
		pjpg = malloc(jpg_size);

		/* 读取jpg文件所有内容到内存 */
		read(jpg_fd, pjpg, jpg_size);
	}
	else
	{
		jpg_size = jpg_buf_size;

		pjpg = pjpg_buf;
	}

	/*注册出错处理*/
	cinfo.err = jpeg_std_error(&jerr);

	/*创建解码*/
	jpeg_create_decompress(&cinfo);

	/*直接解码内存数据*/
	jpeg_mem_src(&cinfo, pjpg, jpg_size);

	/*读文件头*/
	jpeg_read_header(&cinfo, TRUE);

	/*开始解码*/
	jpeg_start_decompress(&cinfo);

	/* 定义color的暂存区,优化显示效果 */
	unsigned int color_buf[cinfo.output_height][cinfo.output_width];


	if (jpg_half)
	{
		x_e	= x_s + (cinfo.output_width / 2);
		y_e	= y  + (cinfo.output_height / 2);


		/*读解码数据*/
		while (cinfo.output_scanline < cinfo.output_height)
		{
			pcolor_buf = g_color_buf;

			/* 读取jpg一行的rgb值 */
			jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&pcolor_buf, 1);

			/* 再读取jpg一行的rgb值 */
			jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&pcolor_buf, 1);

			for (i = 0; i < (cinfo.output_width / 2); i++)
			{
				/* 获取rgb值 */
				color = 		*(pcolor_buf + 2);
				color = color | *(pcolor_buf + 1) << 8;
				color = color | *(pcolor_buf) << 16;

				/* 显示像素点 */
				lcd_draw_point(x, y, color);


				pcolor_buf += 6;

				x++;
			}

			/* 换行 */
			y++;
			x = x_s;
		}

	}
	else
	{
		x_e	= x_s + cinfo.output_width;
		y_e	= y  + cinfo.output_height;

		/*读解码数据*/
		while (cinfo.output_scanline < cinfo.output_height)
		{
			pcolor_buf = g_color_buf;

			/* 读取jpg一行的rgb值 */
			jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&pcolor_buf, 1);

			for (i = 0; i < cinfo.output_width; i++)
			{
				/* 获取rgb值 */
				color = 		*(pcolor_buf + 2);
				color = color | *(pcolor_buf + 1) << 8;
				color = color | *(pcolor_buf) << 16;

				/* 显示像素点 */
				lcd_draw_point(x, y, color);
				pcolor_buf += 3;

				x++;
			}

			/* 换行 */
			y++;

			x = x_s;
		}
	}




	/*解码完成*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	if (pjpg_path != NULL)
	{
		/* 关闭jpg文件 */
		close(jpg_fd);

		/* 释放jpg文件内存空间 */
		free(pjpg);
	}



	return 0;
}

int lcd_draw_jpg_in_jpg(unsigned int x, unsigned int y, const char *pjpg_path, char *pjpg_buf, unsigned int jpg_buf_size)
{
	/*定义解码对象，错误处理对象*/
	struct 	jpeg_decompress_struct 	cinfo;
	struct 	jpeg_error_mgr 			jerr;

	char 	*pcolor_buf = g_color_buf;
	char 	*pjpg;

	unsigned int 	i = 0, j = 0;
	unsigned int	color = 0;
	unsigned int	count = 0;

	unsigned int 	x_s = x;
	unsigned int 	x_e;
	unsigned int 	y_e;
	unsigned int	y_n	= y;
	unsigned int	x_n	= x;

	int	jpg_fd;
	unsigned int 	jpg_size;

	if (pjpg_path != NULL)
	{
		/* 申请jpg资源，权限可读可写 */
		jpg_fd = open(pjpg_path, O_RDWR);

		if (jpg_fd == -1)
		{
			printf("open %s error\n", pjpg_path);

			return -1;
		}

		/* 获取jpg文件的大小 */
		jpg_size = file_size_get(pjpg_path);

		/* 为jpg文件申请内存空间 */
		pjpg = malloc(jpg_size);

		/* 读取jpg文件所有内容到内存 */
		read(jpg_fd, pjpg, jpg_size);
	}
	else
	{
		jpg_size = jpg_buf_size;

		pjpg = pjpg_buf;
	}

	/*注册出错处理*/
	cinfo.err = jpeg_std_error(&jerr);

	/*创建解码*/
	jpeg_create_decompress(&cinfo);

	/*直接解码内存数据*/
	jpeg_mem_src(&cinfo, pjpg, jpg_size);

	/*读文件头*/
	jpeg_read_header(&cinfo, TRUE);

	/*开始解码*/
	jpeg_start_decompress(&cinfo);


	x_e	= x_s + cinfo.output_width;
	y_e	= y  + cinfo.output_height;

	/*读解码数据*/
	while (cinfo.output_scanline < cinfo.output_height)
	{
		pcolor_buf = g_color_buf;

		/* 读取jpg一行的rgb值 */
		jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&pcolor_buf, 1);

		for (i = 0; i < cinfo.output_width; i++)
		{
			/* 不显示的部分 */
			if (y_n > g_jpg_in_jpg_y && y_n < g_jpg_in_jpg_y + 240) if (x_n > g_jpg_in_jpg_x && x_n < g_jpg_in_jpg_x + 320)
				{
					pcolor_buf += 3;

					x_n++;

					continue;
				}

			/* 获取rgb值 */
			color = 		*(pcolor_buf + 2);
			color = color | *(pcolor_buf + 1) << 8;
			color = color | *(pcolor_buf) << 16;

			/* 显示像素点 */
			lcd_draw_point(x_n, y_n, color);

			pcolor_buf += 3;

			x_n++;
		}

		/* 换行 */
		y_n++;

		x_n = x_s;

	}


	/*解码完成*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	if (pjpg_path != NULL)
	{
		/* 关闭jpg文件 */
		close(jpg_fd);

		/* 释放jpg文件内存空间 */
		free(pjpg);
	}



	return 0;
}
#endif

/**
 * draw background
 * 
 * @author gec (15/01/19)
 * 
 * @param addr 
 * @param color 
 */
void draw_backgroud(int color)
{
	//write
	printf("========draw_bg start=====\n");
	int row, col;
	for (row = 0; row < LCD_HEIGHT; row++)
	{
		for (col = 0; col < LCD_WIDTH; col++)
		{
			*(g_pfb_memory + row * LCD_WIDTH + col) = color;
		}
	}
	printf("========draw_bg end=====\n");

}


int lcd_draw_bmp_with_start(const char *image_path, int start_x, int start_y)
{
	//open image
	printf("draw image start %d,%d\n", start_x, start_y);
//	start_y = LCD_HEIGHT - start_y;
	int fd_image = open(image_path, O_RDONLY);
	if (-1 == fd_image)
	{
		perror("open image failed");
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

	//wait for optimize
	int end_x, end_y;
//	start_x = LCD_WIDTH / 2 - width / 2;
//	start_y = LCD_HEIGHT / 2 - height / 2;
	end_x = min(start_x + width, LCD_WIDTH);
	end_y = min(start_y + height, LCD_HEIGHT);

	//output
	int x, y;
	int color_index;
	for (y = start_y; y < end_y; y++)
	{
		for (x = start_x; x < end_x; x++)
		{
			//printf("(%d,%d)\n",x,y);
			color_index = (y - start_y) * width + x - start_x;
			*(g_pfb_memory + (height - (y - start_y) + start_y) * LCD_WIDTH + x) =
				buffer[color_index * 3 + 0] << 0 |
				buffer[color_index * 3 + 1] << 8 |
				buffer[color_index * 3 + 2] << 16;
		}
	}

	printf("\n");

	close(fd_image);
	return 0;
}

//LCD任意地址绘制图片
int lcd_draw_bmp(const char *image_path)
{
	draw_backgroud(0x00000);
	//open image
	int fd_image = open(image_path, O_RDONLY);
	if (-1 == fd_image)
	{
		perror("open image failed");
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

	//wait for optimize
	int start_x, start_y;
	int end_x, end_y;
	start_x = LCD_WIDTH / 2 - width / 2;
	start_y = LCD_HEIGHT / 2 - height / 2;
	end_x = LCD_WIDTH / 2 + width / 2;
	end_y = LCD_HEIGHT / 2 + height / 2;

	//output
	int x, y;
	int color_index;
	for (y = start_y; y < end_y; y++)
	{
		for (x = start_x; x < end_x; x++)
		{
			//printf("(%d,%d)\n",x,y);
			color_index = (y - start_y) * width + x - start_x;
			*(g_pfb_memory + (LCD_HEIGHT - 1 - y) * LCD_WIDTH + x) =
				buffer[color_index * 3 + 0] << 0 |
				buffer[color_index * 3 + 1] << 8 |
				buffer[color_index * 3 + 2] << 16;
		}
	}

	printf("\n");

	close(fd_image);
}

//LCD关闭
void lcd_close(void)
{

	/* 取消内存映射 */
	munmap(g_pfb_memory, FB_SIZE);

	/* 关闭LCD设备 */
	close(g_fb_fd);
}

void draw_image(char *image_path)
{
	if (strstr(image_path, ".bmp"))
	{
		printf("bmp type\n");
		lcd_draw_bmp(image_path);
	}
	else if (strstr(image_path, "jpg"))
	{
		printf("jpg type\n");
		if (strstr(image_path, "image")) /* 符合相机拍照规则 */
		{
			draw_backgroud(0x000000);
			lcd_draw_jpg(80, 0, image_path, NULL, 0, 0);
		}
		else
		{
			lcd_draw_jpg(0, 0, image_path, NULL, 0, 0);
		}
	}
}

void draw_line(struct Point p_start, struct Point p_end, int color)
{
	int x, y;
	for (y = p_start.y; y < p_end.y; y++)
	{
		for (x = p_start.x; x < p_end.x; x++)
		{
			*(g_pfb_memory + (LCD_WIDTH)*y + x) = color;
		}
	}
}

void draw_rect(struct Point start, int width, int height, int color)
{
	int start_x = start.x;
	int start_y = start.y;

	int end_x = min(start_x + width, LCD_WIDTH);
	int end_y = min(start_y + height, LCD_HEIGHT);

	int x, y;
	for (y = start_y; y < end_y; y++)
	{
		for (x = start_x; x < end_x; x++)
		{
			*(g_pfb_memory + (LCD_WIDTH)*y + x) = color;
		}
	}

}


int draw_cross_line(struct Point line, int color)
{
	//绘制第一条线 横轴
	draw_backgroud(0x000000);
	int start_x = 0;
	int start_y = line.y;
	int end_x = LCD_WIDTH;
	int end_y = min(line.y + 10, LCD_HEIGHT);

	int x, y;
	for (x = start_x; x < end_x; x++)
	{
		for (y = start_y; y < end_y; y++)
		{
			*(g_pfb_memory + y * LCD_WIDTH + x) = color;
		}
	}
	//第二条
	start_x = line.x;
	start_y = 0;
	end_x = min(line.x + 10, LCD_WIDTH);
	end_y = LCD_HEIGHT;
	for (x = start_x; x < end_x; x++)
	{
		for (y = start_y; y < end_y; y++)
		{
			*(g_pfb_memory + y * LCD_WIDTH + x) = color;
		}
	}
}
