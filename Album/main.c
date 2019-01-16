/**
 * 相册主程序
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd_info.h"
#include "touch.h"
#include "memory_share.h"
#include "dir.h"
#include "lcd.h"
#include "album.h"

static int fd_touch = -1;


void init()
{
    /* touch init */
    fd_touch = open("/dev/input/event0",O_RDONLY);
    if(-1==fd_touch)
    {
        perror("init failed");
        return ;
    }

    int image_num = -1;
    char *image_path[50];
    int i;
    for(i = 0; i < 50; i++)
    {
        image_path[i] = (char *)malloc(sizeof(char) * 50);
    }
    int read_ret = read_files("./Image", "image", image_path, &image_num);
    if(-1 == read_ret)
    {
        printf("read image files failed\n");
        exit(0);
    }
    /* lcd draw init */
    lcd_open();
    //初始化相册
    init_album( image_path, image_num);
    printf("main init finished\n");
}


void destory()
{
    close(fd_touch);
    lcd_close();
}

int main(int argc, char const *argv[])
{
    //init
    init();


    int i = 0;

    //show a pic
    //image_show(lcd_addr,image_path[i%image_num]);

    int x,y;
    int start_x,start_y;
    int end_x,end_y;
    int threshold = 20; //判决门限
    int delta_x,delta_y;
    //滑动换图
    struct input_event event;
    while(1)
    {
        while(1)
        {
            read(fd_touch,&event,sizeof(event));
            if(EV_ABS == event.type)
            {
                if(ABS_X == event.code)
                {
                    x
                        = event.value;
                }
                else if(ABS_Y == event.code)
                {
                    y = event.value;
                }
            }
            if(EV_KEY == event.type)
            {
                if(BTN_TOUCH == event.code && 1 ==event.value)
                {
                    start_x = x;
                    start_y = y;
                }else if(BTN_TOUCH == event.code && 0 == event.value)
                {
                    end_x = x;
                    end_y = y;
                    break;
                }
            }
        }
        delta_x = start_x - end_x;
        if(delta_x >threshold)
        {
            next_image();
        }
        else if(delta_x < -threshold)
        {
            pre_image();
        }
        printf("\n");
    }
    destory();
    return 0;
}

