#include "voice_common.h"
#include "album.h"
#include "music.h"
#include "video.h"
#include "scheduler.h"
#include <pthread.h>
#include "album_main.h"
#include "voice_recon_main.h"
#include "lcd.h"
#include "music_player_main.h"
#include "video_player_main.h"

#define REC_CMD  "arecord -d3 -c1 -r16000 -traw -fS16_LE cmd.pcm"
#define PCM_FILE "./cmd.pcm"

/**
 * <cmd>:播放音乐!id(1)|上一首!id(2)|下一首!id(3)|音乐暂停!id(10)| 
 * 视频播放!id(4)|上一部!id(5)|下一部!id(6)| 
 * 视频暂停!id(7)|相册!id(8)|上一张!id(9)|下一张!id(10)|退出!id(999); 
 */
/* 定义语音识别码 */
/**
 * 	MUSIC
 */
#define START_MUSIC 0
#define PLAY_MUSIC 1
#define PRE_MUSIC 2
#define NEXT_MUSIC 3
#define PAUSE_MUSIC 10
/**
 * VIDEO
 */
#define START_VIDEO 12
#define PLAY_VIDEO 4
#define PRE_VIDEO 5
#define NEXT_VIDEO 6
#define PAUSE_VIDEO 7
#define FOR_WARD_VIDEO 15
#define BACK_WARD_VIDEO 16

/**
 * ALBUM
 */
#define START_ALBUM 8
#define PRE_PHOTO 9
#define NEXT_PHOTO 11
#define EXIT 999


/* create threads */
pthread_t music_thread;
pthread_t album_thread;
pthread_t video_thread;
pthread_t voice_thread;
int is_music_start = 0, is_album_start = 0, is_video_start = 0; /* 功能辅助变量 */
int is_exit = 0;


/**
 * clear all running thread
 * 
 * @author gec (15/12/16)
 */
static clear_thread()
{
	if (is_music_start)
	{
		is_music_start = 0;
		if (0 != pthread_cancel(music_thread))
		{
			printf("cancel music thread failed\n");
		}
		else
		{
			printf("cancel video thread success\n");
		}
	}
	if (is_album_start)
	{
		is_album_start = 0;
		printf("cancel album thread\n");
		if (0 != pthread_cancel(album_thread))
		{
			printf("cancel album thread failed\n");
		}
		else
		{
			printf("cancel album thread success\n");
		}

	}
	if (is_video_start)
	{
		is_video_start = 0;
		video_out_destory();
		if (0 != pthread_cancel(video_thread))
		{
			printf("cancel video thread failed\n");
		}
		else
		{
			printf("cancel video thread success\n");
		}
		printf("function = %s,line = %d\n", __FUNCTION__, __LINE__);
	}
	return  0;
}


int back2voice()
{
	printf("back to voice\n");
	lcd_open();
	draw_image("./Image/voice_control.bmp");
	clear_thread();
	return 0;
}



/***********************相册相关****************************************/
static void* album_thread_event()
{
	album_main(VOICE_RECON);
	return (void *)0;
}

static int start_album_thread()
{
	clear_thread();

	int ret = pthread_create(&album_thread, NULL, album_thread_event, NULL);
	if (ret)
	{
		perror("create album thread error!");
		return -1;
	}
	is_album_start = 1;
	return 0;
}


/********************start_music_thread*******************/
static void* muisc_thread_event()
{
	music_player_main(VOICE_RECON);
	return (void *)0;
}


static int start_music_thread()
{
	clear_thread();

	int ret = pthread_create(&music_thread, NULL, muisc_thread_event, NULL);
	if (ret)
	{
		perror("create album thread error!");
		return -1;
	}
	is_music_start = 1;
	return 0;
}

/**********************視頻相關**************************************/
static void* video_thread_event()
{
	video_player_main(VOICE_RECON);
	return (void *)0;
}

static int start_video_thread()
{
	clear_thread();

	int ret = pthread_create(&video_thread, NULL, video_thread_event, NULL);
	if (ret)
	{
		perror("create video thread error!");
		return -1;
	}
	is_video_start = 1;
	return 0;
}




int sockfd = -1;
static void* voice_thread_event()
{
	int id_num = 0, num;
	int is_music_played = 0;
	printf("waiting for result...\n");
	while (1)
	{
		// 1，调用arecord来录一段音频
		printf("input num to start REC in 3s...\n");
		scanf("%d", &num);
		printf("======start record voice======\n");
		//启动录音
		system(REC_CMD);

		// 2，将录制好的PCM音频发送给语音识别引擎
		send_pcm(sockfd, PCM_FILE);

		// 3，等待对方回送识别结果（字符串ID）
		xmlChar *id = wait4id(sockfd);
		if (id == NULL) continue;

		//将字符串的id转化成为整形的id
		id_num = atoi((char *)id);
		printf("%d\n", id_num);

		switch (id_num)
		{
			/****************MUSIC*****************/
		case START_MUSIC:
			printf("start music\n");
			if (!is_music_start)
			{
				start_music_thread();
			}
		case PLAY_MUSIC:
			printf("play music\n");
			if (is_music_start)
			{
				pause_or_play();
			}
			break;
		case PAUSE_MUSIC:
			printf("pause music\n");
			if (is_music_start)
			{
				pause_or_play();
			}
			break;
		case PRE_MUSIC:
			printf("pre music\n");
			if (is_music_start)
			{
				pre_music();
			}
			break;
		case NEXT_MUSIC:
			printf("next music\n");
			if (is_music_start)
			{
				next_music();
			}
			break;

			/****************MUSIC*****************/

			/****************VIDEO*****************/
		case START_VIDEO:
			printf("start video\n");
			if (!is_video_start)
			{
				start_video_thread();
			}
			break;
		case PLAY_VIDEO:
			printf("play video\n");
			if (is_video_start)
			{
				play_or_pause();
			}
			break;
		case PRE_VIDEO:
			printf("pre video'\n");
			if (is_video_start)
			{
				pre_video();
			}
			break;
		case NEXT_VIDEO:
			printf("next video\n");
			if (is_video_start)
			{
				next_video();
			}
			break;
		case PAUSE_VIDEO:
			printf("pause video\b");
			if (is_video_start)
			{
				play_or_pause();
			}
			break;
		case FOR_WARD_VIDEO:
			printf("forward video\n");
			if (is_video_start)
			{
				forward(5);
			}
			break;
		case BACK_WARD_VIDEO:
			printf("back ward video\n");
			if (is_video_start)
			{
				backward(5);
			}
			break;
			/****************VIDEO*****************/

			/****************ALBUM*****************/
		case START_ALBUM:
			printf("start album\n");
			if (!is_album_start)
			{
				start_album_thread();
			}
			break;
		case PRE_PHOTO:
			printf("pre photo\n");
			if (is_album_start)
			{
				pre_image();
			}
			break;
		case NEXT_PHOTO:
			printf("next phtoto\n");
			if (is_album_start)
			{
				next_image();
			}
			break;

			/****************ALBUM*****************/
		case EXIT:
			printf("exit voice control!!!\n");
			is_exit = 1;
			break;
		}
	}
}

static int init()
{
	/* 初始化 */
	lcd_open();

	draw_image("./Image/voice_control.bmp");

	sockfd = init_sock("172.16.1.3"); //由命令行传入一个对方的IP 等效于socket+bind+listen+accept

	/* 初始化輔助變量集合 */
	is_music_start = 0;
	is_album_start = 0;
	is_video_start = 0;
	is_exit = 0;



	/* voice thread */
	pthread_create(&voice_thread, NULL, voice_thread_event, NULL);

	printf("voice control init\n");
	return 0;
}



static int destory()
{
	pthread_cancel(voice_thread);
	printf("FUNC = %s ,LINE=%d\n", __FUNCTION__, __LINE__);
	clear_thread();
	printf("FUNC = %s ,LINE=%d\n", __FUNCTION__, __LINE__); 
	close(sockfd);
	printf("FUNC = %s ,LINE=%d\n", __FUNCTION__, __LINE__); 
	lcd_close();
	printf("FUNC = %s ,LINE=%d\n", __FUNCTION__, __LINE__); 
}


int voice_recon_main() // ./wav2pcm ubuntu-IP
{
	init();
	while (!is_exit);
	destory();
	condition = MENU2;
	return 0;
}


