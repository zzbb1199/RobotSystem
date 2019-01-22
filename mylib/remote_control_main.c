#include "remote_control_main.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "lcd.h"
#include "touch.h"
#include "scheduler.h"
#include <socket.h>
#include "music_player_main.h"
#include "video_player_main.h"
#include "api_v4l2.h"

/**
 * 定义功能识别码
 */
#define CAMERA_PLAY "GET_VIDEO"
#define MUSIC_PLAY "ENTER_KUGOU"
#define VIDEO_PLAY  "MUSIC_STOP_CONT"
#define BACK 	"BACKR_KUGOU"
#define PRE_EVENT "MUSIC_PREV_CONT"
#define NEXT_EVENT "MUSIC_NEXT_CONT"
#define PLAY_OR_PUASE "MUSIC_PLAY_CONT"
#define LED_ON "LED_ON"
#define LED_OFF "LED_OFFLED_OFF"
#define BEEP "BEEPOFFLED_OFF"


/**
 * 定义socket相关
 */

static int fd_my_socket = -1; /* 我的邮箱地址 */

static struct sockaddr_in my_addr;  /* 我的地址 */

static struct sockaddr_in other_addr;     /* 手机的地址 */

static int addrlen  = -1; /* 地址长度 */

static int is_exit = 0; /* 退出辅助变量 */


static int is_camera_start = 0;
static int is_music_start = 0;
static int is_video_start = 0;


static pthread_t receive_thread;
static pthread_t send_camera_thread;
static pthread_t touch_thread;
static pthread_t music_thread;
static pthread_t video_thread;


/***************************清除功能线程******************************************/
static clear_func_thread()
{
//	if (is_camera_start)
//	{
//		is_camera_start = 0;
//		if (0 != pthread_cancel(send_camera_thread))
//		{
//			printf("cancel send_camera thread failed\n");
//		}
//		else
//		{
//			printf("cancel send_camera thread success\n");
//		}
//	}
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

int back2remotecontrol()
{
	printf("back to remote control\n");
	lcd_open();
	draw_image("./Image/remote_control_background.bmp");
	clear_func_thread();
	return 0;
}



/**********************摄像头线程**************************************/

/**
 * 发送摄像头数据
 * 
 * @author gec (21/01/19)
 * 
 * @return void* 
 */
static void* send_camera_thread_event()
{

	FrameBuffer freambuf;

	/* 初始化摄像头设备*/
	linux_v4l2_device_init("/dev/video7");

	/* 启动摄像头*/
	linux_v4l2_start_capturing();
	int ret = -1;
	while (1)
	{
		// 实时监控
		/* 获取摄像头数据      存放jpg文件流*/
		linux_v4l2_get_fream(&freambuf);

		ret = sendto(fd_my_socket, freambuf.buf, freambuf.length,
					 0, (struct sockaddr *)&other_addr, sizeof(struct sockaddr));

		if (-1 == ret)
		{
			perror("send msg error\n");
		}
		else
		{
			printf("send %d bytes\n", ret);
		}
	}

	/* 停止摄像头*/
	linux_v4l2_stop_capturing();

	/* 卸载摄像头*/
	linux_v4l2_device_uinit();


	return (void *)0;
}

static int start_send_camera_thread()
{
	int ret = pthread_create(&send_camera_thread, NULL, send_camera_thread_event, 0);
	if (!ret)
	{
		printf("create send camera thread failed\n");
		return -1;
	}
	is_camera_start = 1;
}

/********************start_music_thread*******************/
static void* muisc_thread_event()
{
	music_player_main(REMOTE_CONTROL);
	return (void *)0;
}


static int start_music_thread()
{
	clear_func_thread();

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
	video_player_main(REMOTE_CONTROL);
	return (void *)0;
}

static int start_video_thread()
{
	clear_func_thread();

	int ret = pthread_create(&video_thread, NULL, video_thread_event, NULL);
	if (ret)
	{
		perror("create video thread error!");
		return -1;
	}
	is_video_start = 1;
	return 0;
}



/**************************接受数据线程**********************************/
static int my_receive(char *msg, int msg_len)
{
	int ret = -1;
	bzero(msg, sizeof(msg));
	printf("===wait data coming====\n");
	ret = recvfrom(fd_my_socket, msg, msg_len, 0,
				   (struct sockaddr *)&other_addr, &addrlen);
	if (-1 == ret)
	{
		perror("receive error");
		return -1;
	}
	printf("receive %d byptes\n", ret);
	return ret;
}

/**
 * 处理接受到的信息
 * 
 * @author gec (22/01/19)
 * 
 * @return int 
 */
static int solve_msg(char *msg)
{

	/** 传输摄像头数据 */
	if (0 == strcmp(msg, CAMERA_PLAY))
	{
		if (!is_camera_start)
		{
			start_send_camera_thread();
		}
	}
	/* 播放音乐 */
	else if (0 == strcmp(msg, MUSIC_PLAY))
	{
		if (!is_music_start)
		{
			start_music_thread();
		}
	}
	/* 视频播放 */
	else if (0 == strcmp(msg, VIDEO_PLAY))
	{
		if (!is_video_start)
		{
			start_video_thread();
		}
	}
	/* BACK */
	else if (0 == strcmp(msg, BACK))
	{}
	/* 上一首或者上一部 */
	else if (0 == strcmp(msg, PRE_EVENT))
	{}
	/* 下一首或者下一部 */
	else if (0 == strcmp(msg, NEXT_EVENT))
	{}
	/* 暂停音乐或者暂停视频 */
	else if (0 == strcmp(msg, PLAY_OR_PUASE))
	{}
	/* LED_ON */
	else if (0 == strcmp(msg, LED_ON))
	{}
	/* LED_OFF */
	else if (0 == strcmp(msg, LED_OFF))
	{}
	/* 蜂鸣器 */
	else if (0 == strcmp(msg, BEEP))
	{}
	return 0;
}

/**
 * 接受事件线程
 * 
 * @author gec (21/01/19)
 * 
 * @return void* 
 */
static void* receive_thread_event()
{
	printf("======start reciving thread====\n");
	char msg[100];
	int ret = -1;
	while (1)
	{
		ret = my_receive(msg, sizeof(msg));
		if (-1 == ret)
		{
			continue;
		}
		/* 输出msg */
		printf("%s\n", msg);
		/**
		 * 根据收到的信息进行处理!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		 */
		solve_msg(msg);
	}

	return (void *)0;
}

/************************触摸屏线程******************************/


/**
 * 手势监控线程
 * 
 * @author gec (21/01/19)
 * 
 * @return void* 
 */
static void* touch_thread_event()
{
	printf("=====================start touch thread===============\n");
	int x, y, delta_x, delta_y;
	const int exit_threshold = 500;
	while (1)
	{
		scroll(&delta_x, &delta_y, &x, &y);
//		printf("%d,%d,deltax %d", x, y, delta_x);
		if (-delta_x > exit_threshold)
		{
			is_exit = 1;
			break;
		}
	}
	return 0;
}
static int init_socket()
{
	/* 创建socket */
	fd_my_socket =  socket(AF_INET, SOCK_DGRAM, 0);
	if (fd_my_socket == -1)
	{
		perror("socket faileed!");
		return -1;
	}
	else
	{
		printf("create socket succcess\n");
	}
	/* 绑定地址 */
	/* 初始化自己的地址 */
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(2234);
	my_addr.sin_addr.s_addr = inet_addr("192.168.1.100");
	addrlen = sizeof(struct  sockaddr_in);
	/* 绑定邮箱socket */
	int ret = bind(fd_my_socket, (struct sockaddr *)&my_addr, addrlen);
	if (-1 == ret)
	{
		perror("bind failed");
		return -1;
	}
	else
	{
		printf("ip binding succeess\n");
	}


	/* 初始化别人的地址 */
//	other_addr.sin_family = AF_INET;
//	other_addr.sin_port = htons(65000);
//	other_addr.sin_addr.s_addr = inet_addr("192.168.1.101");
	return 0;
}

/**
 * 开启各个线程
 * 
 * @author gec (21/01/19)
 * 
 * @return int 
 */
static int init_threads()
{
	/** 开启接收数据线程 */
	int ret = pthread_create(&receive_thread, NULL, receive_thread_event, 0);
	if (ret)
	{
		printf("create receive thread failed\n");
		return -1;
	}
	/* 开启触摸屏线程 */
	ret = pthread_create(&touch_thread, NULL, touch_thread_event, 0);
	if (ret)
	{
		printf("create touch thread failed\n");
		return -1;
	}
	return 0;
}

static int init()
{

	touch_open();
	lcd_open();
	/* 初始化socket */
	init_socket();
	/* 初始化辅助线程开启变量 */
	is_camera_start = 0;
	is_music_start = 0;
	is_video_start = 0;
	is_exit = 0;
	/* 初始化线程 */
	init_threads();

	/* 绘制远程控制背景 */
	draw_image("./Image/remote_control_background.bmp");
	return 0;
}

static int destory()
{
	/* 关闭socket */
	if (-1 != fd_my_socket)
	{
		close(fd_my_socket);
	}
	/* 关闭lcd */
	lcd_close();
	touch_close();
	/* 清除主要线程各个线程 */
	pthread_cancel(receive_thread);
	if (is_camera_start)
	{
		pthread_cancel(send_camera_thread);
		is_camera_start = 0;
	}
	pthread_cancel(touch_thread);
	clear_func_thread();
	return 0;
}


int remote_control_main()
{
	init();
	while (!is_exit);
	destory();
	condition = MENU2;
	return 0;
}
