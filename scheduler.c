/**
 * 功能调度器 
 * 使用堆栈保存前一个功能
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
#include "theme_change_main.h"
#include <stdlib.h>
#include <string.h>

condition = -1;
int theme_choose = THEME1;
int main(void)
{
	/** 初始化theme */
	int fd_theme_file = open("./theme", O_RDONLY | O_CREAT);
	if (-1 == fd_theme_file)
	{
		perror("open theme file error!\n");
	}
	char theme[7];
	bzero(theme, 7);
	read(fd_theme_file, theme, 7);
	printf("theme is %s\n", theme);
	if (strstr(theme, "theme1"))
	{
		printf("theme1\n");
		theme_choose = THEME1;
		system("cp ./Theme1/* ./Image");
	}
	else if (strstr(theme, "theme2"))
	{
		printf("theme2\n");
		theme_choose = THEME2;
		system("cp ./Theme2/* ./Image");
	}
	else
	{
		printf("no theme file\n");
		theme_choose = THEME1;
		system("cp ./Theme1/* ./Image");
		write(fd_theme_file, "theme1", 6);
	}
	close(fd_theme_file);
	usleep(500 * 1000);
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
		case THEME_CHOOSE:
			theme_change_main();
			break;
		default:
			printf("there is no func named that\n");
			printf("function = %s\n", __FUNCTION__);
			break;
		}
	}
	return 0;
}


