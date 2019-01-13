// coordination info

#ifndef __TOUCH_H__
#define __TOUCH_H__
#include <linux/input.h>
#include <unistd.h>

void get_xy(int fd,int *x,int *y);


#endif
