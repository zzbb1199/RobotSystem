/*
* 十字交叉线跟随手指-主程序
*/
#include "lcd.h"
#include <stdio.h>
#include "memory_share.h"
#include <linux/input.h>
#include "touch.h"
#include "public_resource.h"
#include "cross_line_main.h"

static void init()
{
    lcd_open();

    touch_open();
}
static int destory()
{
    lcd_close();

    touch_close();
}



int cross_line_main(void)
{
    //init
    init();
    struct Point line;
    line.x = 100;
    line.y = 100;
    //draw_cross_line(addr,line);

    struct input_event event;
    int x,y;
    int count = 0;
    while(1)
    {
        get_xy( &x, &y);
        line.x = x;
        line.y = y;
        //draw_cross_line(addr, line); 
    }

    destory();
}

