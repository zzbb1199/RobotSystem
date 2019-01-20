#include "music.h"


int init_music()
{
	lcd_open(); 
	//获取音乐文件路径
	read_files("./Music", "mp3", music_path, &music_num);
	//绘制背景图
	draw_image("./Image/music_pause.bmp");
	return 0;
}


int start_music()
{

//	//播放第一首音乐
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
	system("killall -19 madplay");  /*悬挂进程*/
}

static int play_music()
{
	printf("play\n");
	system("killall -18 madplay");  /*继续进程*/
}



int pause_or_play()
{
	if (isplayed)
	{
		draw_image("./Image/music_play.bmp");
		pause_music();
		isplayed = 0;
	}
	else
	{
		draw_image("./Image/music_pause.bmp");
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
	sprintf(cmd, "%s%s%s", "madplay ", music_path[music_i]," &");
	draw_image("./Image/music_pause.bmp");
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
	sprintf(cmd, "%s%s%s", "madplay ", music_path[music_i]," &");
	draw_image("./Image/music_pause.bmp");
	system(cmd);
}
