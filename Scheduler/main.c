/**
 * 功能调度器 
 * 使用堆栈保存前一个功能
 */

#include <stdio.h>
#include "stack.h"
#include "album_main.h"
#include "camera_main.h"
#include "cross_line_main.h"
#include "guaguale_main.h"
#include "menu_main.h"
#include "music_player_main.h"
#include "slide_unlock_main.h"
#include "chat_main.h"
#include "video_player_main.h"




/**
 * 功能宏定义
 */
#define LOCK 0X01
#define MENU 0x02
#define MUSIC_PLAYER 0X03
#define VIDEO_PLAYER 0X04
#define ALBUM 0x05
#define MORE  0x06
#define GUAGUA 0x07
#define CROSS_LINE 0x08


Sqstack fun_stack; /* 功能栈 */

/**
 * 退出到上一级别功能
 * 
 * @author gec (17/01/19)
 * 
 * @return int 
 */
int back_fun();


/**
 *  
 * 走到下一个功能
 * @author gec (17/01/19)
 * 
 * @param fun 
 * 
 * @return int 
 */
int goto_fun(int fun);


int main(void)
{
	return 0;
}

int back_fun()
{
	return 0;
}

int goto_fun(int fun)
{
    {
        switch(fun)
        {
            case LOCK:
                /* 一键锁屏具有特殊性,为默认底层栈 */
                clear_stack(&fun_stack);
                push(&fun_stack, LOCK);
                /* 实际调度 */

                break;
            case MENU:
                break;
            case MUSIC_PLAYER:
                break;
            case VIDEO_PLAYER:
                break;
            case ALBUM:
                break;
            case MORE:
                break;
            case GUAGUA:
                break;
            case CROSS_LINE:
                break;
            default:
                printf("there is no func named that\n");
                printf("function = %s\n", __FUNCTION__);
                break;
        }
    }
	return 0;
}
