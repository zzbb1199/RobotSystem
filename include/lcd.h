#ifndef __LCD_H__
#define __LCD_H__

#define LCD_WIDTH  			800
#define LCD_HEIGHT 			480
#define FB_SIZE				(LCD_WIDTH * LCD_HEIGHT * 4)

#define EN_LCD_SHOW_JPG		1

#include "touch.h"


//初始化LCD
int lcd_open(void);

//LCD画点

void draw_backgroud(int color);

int lcd_draw_jpg(unsigned int x, unsigned int y, const char *pjpg_path, char *pjpg_buf, 
				 unsigned int jpg_buf_size, unsigned int jpg_half);

int lcd_draw_jpg_in_jpg(unsigned int x, unsigned int y,
						 const char *pjpg_path, char *pjpg_buf, unsigned int jpg_buf_size);

/**
 * draw background
 * 
 * @author gec (15/01/19)
 * 
 * @param addr 
 * @param color 
 */
void draw_backgroud(int color);

//LCD任意地址绘制图片
int lcd_draw_bmp(const char *image_path);

//LCD关闭
void lcd_close(void);

void draw_image(char *image_path);


void draw_line(struct Point p_start, struct Point p_end, int color);


void draw_rect(struct Point start, int width, int height, int color);

void lcd_draw_point(unsigned int x, unsigned int y, unsigned int color);

int lcd_draw_bmp_with_start(const char *image_path, int start_x, int start_y);

int draw_cross_line(struct Point line ,int color);
#endif