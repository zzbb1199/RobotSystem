#ifndef __LCD_H__
#define __LCD_H__

#define LCD_WIDTH  			800
#define LCD_HEIGHT 			480
#define FB_SIZE				(LCD_WIDTH * LCD_HEIGHT * 4)

#define EN_LCD_SHOW_JPG		1

struct Point
{
	int x,y;
};
struct Boundary
{
	struct Point *p1,*p2;
};


#endif