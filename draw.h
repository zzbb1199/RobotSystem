//  image show header

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "lcd_info.h"

void draw_bg(int *addr,int color);
/**
 * 输出图片
 */
int image_show(int *lcd,char *image_path);

struct Point
{
    int x;
    int y;
};

struct Boundary
{
	struct Point *p1,*p2;
};
void draw_cross_line(int *addr,struct Point point);
#endif
