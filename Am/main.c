/**
 * 相册主程序
 */

#include <stdio.h>
#include "lcd_info.h"
#include "touch.h"
#include "draw.h"
#include "memory_share.h"
#include "dir.h"

static int *lcd_addr = NULL;
static int fd_touch = -1;

static int image_num = -1;
static char *image_path[50];

void init()
{
    lcd_addr = my_mmap();
    fd_touch = open("/dev/input/event0",O_RDONLY);
    if(-1==fd_touch)
    {
        perror("init failed");
        return ;
    }
    read_files("./image", ".bmp", image_path, &image_num);
}


void destory()
{
    my_munmap(lcd_addr);
    close(fd_touch);
}

int main(int argc, char const *argv[])
{
    //init
    init();


    int i = 0;

    //show a pic
    image_show(lcd_addr,image_path[i%image_num]);

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
                    x = event.value;
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

