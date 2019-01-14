// coordination info

/**
 * 触摸屏操作
 */

#ifndef __TOUCH_H__
#define __TOUCH_H__
#include <linux/input.h>
#include <unistd.h>

/**
 * 获取当前触摸点的横纵坐标
 * 
 * @author raven (19-1-13)
 * 
 * @param fd 触摸板的文件描述符
 * @param x 保存的x
 * @param y 保存的y
 */
void get_xy(int fd,int *x,int *y);


#endif
