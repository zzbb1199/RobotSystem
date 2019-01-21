// coordination info

/**
 * 触摸屏操作
 */

#ifndef __TOUCH_H__
#define __TOUCH_H__
#include <linux/input.h>
#include <unistd.h>
#include <stdio.h>

struct Point
{
	int x,y;
}; 



struct Boundary
{
    struct Point *p1,*p2;
}; 



/**
 * 获取当前触摸点的横纵坐标
 * 
 * @author raven (19-1-13)
 * 
 * @param fd 触摸板的文件描述符
 * @param x 保存的x
 * @param y 保存的y
 */
void get_xy(int *x,int *y);


/**
 * 初始化触摸板资源
 * 
 * @author gec (16/01/19)
 * 
 * @param x 
 * @param y 
 */
int touch_open();


/**
 * 关闭触摸板资源
 * 
 * @author gec (16/01/19)
 * 
 * @return int 
 */
int touch_close();

/**
 * 边界检查
 * 
 * @author gec (15/01/19)
 * 
 * @param x 当前点击区域的横坐标
 * @param y 当前点击区域的纵坐标
 * @param bd 索要检查的限制区域
 * 
 * @return int 
 */
int check_boundary(int x, int y, struct Boundary bd);

int free_boundary(struct Boundary *bd);

/**
 * 华东距离检测
 * 
 * @author gec (16/01/19)
 * 
 * @param delta_x x axis
 * @param delta_y y axis
 * 
 * @return int 
 */
int scroll(int *delta_x, int *delta_y,int *x,int *y);
#endif
