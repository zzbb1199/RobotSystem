/**
 * 功能调度器 
 * 使用堆栈保存前一个功能
 */

#include <stdio.h>
#include "album_main.h"
#include "camera_main.h"
#include "cross_line_main.h"
#include "guaguale_main.h"
#include "menu_main.h"
#include "music_player_main.h"
#include "slide_unlock_main.h"
#include "chat_main.h"
#include "video_player_main.h"
#include "scheduler.h"


int main(void)
{
	int condition = LOCK; /* LOCK as main */
	while (1)
	{
		printf("func %d\n", condition);
		switch (condition)
		{
		case LOCK:
			slide_unlock_main(&condition);
			break;
		case MENU:
			menu_main(&condition);
			break;
		case MUSIC_PLAYER:
			music_player_main(&condition);
			break;
		case VIDEO_PLAYER:
			video_player_main(&condition);
			break;
		case ALBUM:
			album_main(&condition);
			break;
		case MORE:
			break;
		case CAMERA:
            camera_main(&condition);
			break;
		case GUAGUA:
			guaguale_main(&condition);
			break;
		case CROSS_LINE:
			cross_line_main(&condition);
			break;
		default:
			printf("there is no func named that\n");
			printf("function = %s\n", __FUNCTION__);
			break;
		}
	}
	return 0;
}


