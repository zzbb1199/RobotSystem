#include "music.h"
#include "font.h"
#include "lcd.h"
#include "scheduler.h"


#define LITTER_ZOOM_SIZE 2
#define EVERY_LITTER_SIZE 16*LITTER_ZOOM_SIZE

static char current_name[20];

static int draw_music_name(char *name)
{
	
	int music_name_x_middle = 0 ,music_name_y_middle =0 ;
	if (theme_choose == THEME1)
	{
		music_name_x_middle = 242;
		music_name_y_middle = 332;
	}
	else if (theme_choose == THEME2)
	{
		music_name_x_middle = 210;
		music_name_y_middle = 301;
	}

	int name_len = strlen(name);
	strcpy(current_name, name);
	unsigned int start_x = music_name_x_middle -  name_len / 3 * EVERY_LITTER_SIZE;
	unsigned int start_y = music_name_y_middle - (int)EVERY_LITTER_SIZE / 2;
	Display_characterX(start_x, start_y, name, 0x000000, LITTER_ZOOM_SIZE);
	return 0;
}

/**
 * 去掉path,得到音乐名字
 *
 * @author gec (21/01/19)
 *
 * @param path
 *
 * @return int
 */
static int de_music_path(char *path,char *name)
{
	char *s = strstr(path, ".mp3");
	char *save = s;
	while (*(s) != '/')
	{
		s--;
	}
	s++;
	while (s != save)
	{
		*(name++) = *(s++);
	}
	printf("music name %s\n", name);
	return 0;
}

int init_music()
{
	lcd_open();
	//获取音乐文件路径
	read_files("./Music", "mp3", music_path, &music_num);

	Init_Font();
	return 0;
}


int start_music()
{

//	//播放第一首音乐
	//绘制背景图
	draw_image("./Image/music_pause.jpg");
	draw_image("./Image/music_pause.jpg");

	char name[20];
	bzero(name, 20);

	de_music_path(music_path[0], name);
	draw_music_name(name);

	char cmd[50] = "madplay ";
	strcat(cmd, music_path[0]);
	strcat(cmd, " &");
	printf("system call:%s\n", cmd);
	system(cmd);    /*播放*/
	isplayed = 1;
	return 0;
}

static int pause_music()
{
	printf("pause\n");
	draw_music_name(current_name);
	system("killall -19 madplay");  /*悬挂进程*/
}

static int play_music()
{
	printf("play\n");
	draw_music_name(current_name);
	system("killall -18 madplay");  /*继续进程*/
}



int pause_or_play()
{
	if (isplayed)
	{
		draw_image("./Image/music_play.jpg");
		pause_music();
		isplayed = 0;
	}
	else
	{
		draw_image("./Image/music_pause.jpg");
		play_music();
		isplayed = 1;
	}
}


int pre_music()
{
	printf("pre_music\n");
	system("killall -9 madplay");       /*结束还在播放的进程，这里还有bug，需要判定以下是否有播放进程存在*/
	music_i =  (music_i - 1 + music_num) % music_num;   /*得到上一个音乐index*/
	char cmd[30];
	bzero(cmd, 30);
	printf("current music%s\n", music_path[music_i]);
	sprintf(cmd, "%s%s%s", "madplay ", music_path[music_i], " &");
	draw_image("./Image/music_pause.jpg");
	/* 画歌名 */
	char name[20];
	bzero(name, 20);
	de_music_path(music_path[music_i], name);
	draw_music_name(name);
	system(cmd);
}

int next_music()
{
	printf("next_music\n");
	system("killall -9 madplay");       /*结束还在播放的进程，这里还有bug，需要判定以下是否有播放进程存在*/
	music_i =  (music_i + 1) % music_num;   /*得到下一个音乐index*/
	char cmd[30];
	bzero(cmd, 30);
	printf("current music%s\n", music_path[music_i]);
	sprintf(cmd, "%s%s%s", "madplay ", music_path[music_i], " &");
	draw_image("./Image/music_pause.jpg");

	/* 画歌名 */
	char name[20];
	bzero(name, 20);
	de_music_path(music_path[music_i], name);
	draw_music_name(name);

	system(cmd);
}

int destory_music()
{
	lcd_close();
	UnInit_Font();
	/* 释放播放名字的内存 */
	int i;
	for (i = 0; i < music_num; i++)
	{
		if (music_path[i])
		{
			free(music_path[i]);
		}
	}
	/* kill */
	system("killall -9 madplay");
}
