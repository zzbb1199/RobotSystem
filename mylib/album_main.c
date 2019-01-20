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
#include "public_resource.h"
#include "scheduler.h"
#include "voice_recon_main.h"


static void init()
{
 
    /* lcd draw init */
    lcd_open();
    /* touch open */
    touch_open();
    //初始化相册
    init_album( image_path, image_num);
    printf("main init finished\n");
}


static void destory()
{
    touch_close();
    lcd_close();
}


int album_main(int comefrom)
{
    //init
    init();


    int i = 0;

    //show a pic
    //image_show(lcd_addr,image_path[i%image_num]);

    int x,y;
    int threshold = 20; //判决门限
    int exit_threshold = 600;   /* 退出判定 */
    int delta_x,delta_y;
    //滑动换图
    while(1)
    {
        scroll(&delta_x, &delta_y, &x, &y);
        if(delta_x >threshold)
        {
            next_image();
        }
        else if(delta_x < -threshold)
        {
            pre_image();
        }

        if(-delta_x >= exit_threshold)
        {
            break;
        }
        printf("\n");
    }
    destory();
    if(VOICE_RECON == comefrom)
    {
        printf("comefrom VOICE\n");
        condition = VOICE_RECON;
        back2voice();
    }
    else
    {
        condition = MENU;
    }
    return 0;
}

