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
#include "voice_recon_main.h"
#include "lock_style_main.h"
#include "remote_control_main.h"


condition = -1;
int main(void)
{
	condition = LOCK; /* LOCK as main */
	while (1)
	{
		printf("============goto func======== %d\n", condition);
		switch (condition)
		{
		case LOCK:
			slide_unlock_main();
			break;
		case MENU:
			menu_main();
			break;
		case MUSIC_PLAYER:
			music_player_main(MENU);
			break;
		case VIDEO_PLAYER:
			video_player_main(MENU);
			break;
		case ALBUM:
			album_main(MENU);
			break;
		case CAMERA:
			camera_main();
			break;
		case GUAGUA:
			guaguale_main();
			break;
		case CROSS_LINE:
			cross_line_main();
			break;
		case MENU2:
			menu2_main();
			break;
		case CHAT:
			printf("conditon = %d\n", condition);
			chat_main();
			break;
		case REMOTE_CONTROL:
			remote_control_main();
			break;
		case VOICE_RECON:
			voice_recon_main();
			break;
		case LOCK_STYLE:
			lock_style_main();
			break;
		default:
			printf("there is no func named that\n");
			printf("function = %s\n", __FUNCTION__);
			break;
		}
	}
	return 0;
}


