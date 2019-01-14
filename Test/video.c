#include "video.h"



static char *video_path[50]; /*视频文件路径*/

static int i = 0;   /*当前视频index*/

static int isplayed = -1;   /*是否正在播放*/


static int send_cmd(char *cmd)
{
	(-1 == write(fd_fifo, cmd, strlen(cmd)))
	{
		printf("write video conrol cmd error");
		return -1;
	}
	return 0;
}

int init_video(char *name[], int num, int fifo)
{
	if(0 == num)
	{
		printf("current video nums is 0!");
		return -1;
	}
	int i;
	for(i =  0i;<nmu; i++)
	{
		video_path[i] = name[i];
	}
	video_num = num;
	
	fd_fifo = fifo;
	
	//开始播放第一个视频
	char cmd[100];
	bzero(cmd, 100);
	sprintf(cmd, "%s%s", "mplayer -slave -input file=/tmp/fifo -quiet -zoom -x 800 -y 480 ",
			video_path[0]);
	system(cmd);
	return 0;
}

int play_or_pause()
{
	send_cmd("mpalyer pause");
	return 0;
}

int forward(int sec)
{
	return 0;
}

int backward(int sec);
{
	return 0;
}


int pre_video()
{
	printf("pre_video\n");
	system("killall -9 mplayer");       /*结束还在播放的进程，这里还有bug，需要判定以下是否有播放进程存在*/
	i =  (i - 1 + video_num) % video_num;   /*得到上一个音乐index*/
	char cmd[100];
	bzero(cmd, 100);
	printf("current video%s\n", video_path[i]);
	sprintf(cmd, "%s%s%s", "mplayer ", video_path[i], "-quiet -zoom -x 800 -y 480 &");
	system(cmd);
}



int next_video()
{
	printf("next_video\n");
	system("killall -9 mplayer");       /*结束还在播放的进程，这里还有bug，需要判定以下是否有播放进程存在*/
	i =  (i + 1) % video_num;   /*得到下一个音乐index*/
	char cmd[100];
	bzero(cmd, 100);
	printf("current video%s\n", video_path[i]);
	sprintf(cmd, "%s%s%s", "mplayer ", video_path[i], "-quiet -zoom -x 800 -y 480 &");
	system(cmd);
}


