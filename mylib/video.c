#include "video.h"


static int send_cmd(char *cmd)
{
	printf("cmd = %s", cmd);
	int ret = write(fd_fifo, cmd, strlen(cmd));
	if (-1 == ret)
	{
		printf("write video conrol cmd error");
		return -1;
	}
	return 0;
}

int init_video(char *name[], int num, int fifo)
{
	if (0 == num)
	{
		printf("current video nums is 0!");
		return -1;
	}
	int i;
	for (i =  0; i < num; i++)
	{
		video_path[i] = name[i];
	}
	video_num = num;

	fd_fifo = fifo;

	//开始播放第一个视频
	char cmd[100];
	bzero(cmd, 100);
	sprintf(cmd, "%s%s%s", "mplayer -slave -input file=/tmp/fifo -quiet -zoom -x 800 -y 480 ",
			video_path[0], " &");
	printf("current cmd:%s\n", cmd);
	system(cmd);
	send_cmd("loop 0\n");       /**/
	return 0;
}

int play_or_pause()
{
	send_cmd("pause\n");
	return 0;
}

int forward(int sec)
{
	char cmd[20];
	sprintf(cmd, "%s%d%s", "seek +", sec, "\n");
	send_cmd(cmd);
	return 0;
}

int backward(int sec)
{
	char cmd[20];
	sprintf(cmd, "%s%d%s", "seek ", sec, "\n");
	send_cmd(cmd);
	return 0;
}


/**
 * 音量加
 *
 * @author gec (14/01/19)
 *
 * @param level 音量加多少
 *
 * @return int
 */
int volume_up(float level)
{
	char cmd[20];
	sprintf(cmd, "%s%f%s", "volume ", level, "\n");
	send_cmd(cmd);
	return 0;
}
/**
 * 音量减少
 *
 * @author gec (14/01/19)
 *
 * @param level  音量减少多少
 *
 * @return int
 */
int volume_down(float level)
{
	char cmd[20];
	sprintf(cmd, "%s%f%s", "volume ", level, "\n");
	send_cmd(cmd);
}




int pre_video()
{
	printf("pre_video\n");
	system("killall -15 mplayer");       /*结束还在播放的进程，这里还有bug，需要判定以下是否有播放进程存在*/
	i =  (i - 1 + video_num) % video_num;   /*得到上一个音乐index*/
	char cmd[100];
	bzero(cmd, 100);
	printf("current video%s\n", video_path[i]);
	sprintf(cmd, "%s%s%s", "mplayer ", video_path[i], " -slave -input file=/tmp/fifo -quiet -zoom -x 800 -y 480 &");
	system(cmd);
}



int next_video()
{
	printf("next_video\n");
	system("killall -15 mplayer");       /*结束还在播放的进程，这里还有bug，需要判定以下是否有播放进程存在*/
	i =  (i + 1) % video_num;   /*得到下一个音乐index*/
	char cmd[100];
	bzero(cmd, 100);
	printf("current video%s\n", video_path[i]);
	sprintf(cmd, "%s%s%s", "mplayer ", video_path[i], " -slave -input file=/tmp/fifo -quiet -zoom -x 800 -y 480 &");
	system(cmd);
}



