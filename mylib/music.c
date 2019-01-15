#include "music.h"


int init_music(char *path[], int num)
{
	int i;
	for (i = 0; i < num;i++)
	{
		music_path[i] = path[i];
	}
	music_num = num;
	//播放第一首音乐
	char cmd[50] = "madplay ";
	strcat(cmd, music_path[0]);
	strcat(cmd, " &");
	printf("system call:%s\n", cmd);
	system(cmd);	/*播放*/
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
		pause_music();
		isplayed = 0;
	}
	else
	{
		play_music();
		isplayed = 1;
	}
}


int pre_music()
{
	printf("pre_music\n");
	system("killall -9 madplay");       /*结束还在播放的进程，这里还有bug，需要判定以下是否有播放进程存在*/
	i =  (i - 1 + music_num) % music_num;   /*得到上一个音乐index*/
	char cmd[30];
	bzero(cmd, 30);
	printf("current music%s\n", music_path[i]);
	sprintf(cmd, "%s%s%s", "madplay ", music_path[i]," &");
	system(cmd);
}

int next_music()
{
	printf("next_music\n");
	system("killall -9 madplay");       /*结束还在播放的进程，这里还有bug，需要判定以下是否有播放进程存在*/
	i =  (i + 1) % music_num;   /*得到下一个音乐index*/
	char cmd[30];
	bzero(cmd, 30);
	printf("current music%s\n", music_path[i]);
	sprintf(cmd, "%s%s%s", "madplay ", music_path[i]," &");
	system(cmd);
}
