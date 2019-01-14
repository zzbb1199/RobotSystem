/*
* 十字交叉线跟随手指-主程序
*/
#include "draw.h"
#include <stdio.h>
#include "memory_share.h"
#include <linux/input.h>
#include "touch.h"

int *addr = NULL;
int fd_lcd = -1;
int fd_touch = -1;
void init()
{
    addr = my_mmap();
    fd_lcd = open("/dev/fb0",O_RDWR);
    fd_touch = open("/dev/input/event0",O_RDONLY);
    if(-1 == fd_lcd || -1==fd_touch)
    {
        perror("init failed");
        return ;
    }
}
int destory()
{
    my_munmap(addr);
    close(fd_touch);
}



int main(void)
{
    //init
    init();
    struct Line line;
    line.x = 100;
    line.y = 100;
    draw_cross_line(addr,line);

    struct input_event event;
    int x,y;
    int count = 0;
    while(1)
    {
        get_xy(fd_touch, &x, &y);
        line.x = x;
        line.y = y;
        draw_cross_line(addr, line); 
    }

    destory();
}

