#include "music.h"


int init_music(char *path[], int num)
{
	int i;
	for (i = 0; i < num;i++)
	{
		music_path[i] = path[i];
	}
	music_num = num;
	return 0;
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

static int pause_music()
{
	system("killall -19 madplay");  /*悬挂进程*/
}

static int play_music()
{
	system("killall -18 madplay");  /*继续进程*/
}

int pre_music()
{
	system("killall -9 madplay");       /*结束还在播放的进程，这里还有bug，需要判定以下是否有播放进程存在*/
	i =  (i - 1 + music_num) % music_num;   /*得到上一个音乐index*/
	char cmd[30];
	bzero(cmd, 30);
	sprintf(cmd, "%s%s", "madplay ", music_path[i]);
	system(cmd);
}

int next_music()
{
	system("killall -9 madplay");       /*结束还在播放的进程，这里还有bug，需要判定以下是否有播放进程存在*/
	i =  (i + 1) % music_num;   /*得到下一个音乐index*/
	char cmd[30];
	bzero(cmd, 30);
	sprintf(cmd, "%s%s", "madplay ", music_path[i]);
	system(cmd);
}
