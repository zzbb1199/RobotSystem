#ifndef __LCD_H__
#define __LCD_H__

#define LCD_WIDTH  			800
#define LCD_HEIGHT 			480
#define FB_SIZE				(LCD_WIDTH * LCD_HEIGHT * 4)


#define EN_LCD_SHOW_JPG		1

/* video_chat.c 画中画显示的坐标 */
extern volatile int g_jpg_in_jpg_x;
extern volatile int g_jpg_in_jpg_y;


#endif