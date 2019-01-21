#include "remote_control_main.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "lcd.h"
#include "touch.h"
#include "scheduler.h"
#include <socket.h>
/**
 * 定义功能识别码
 */
#define CAMERA_PLAY 0
#define MUSIC_PLAY 1
#define VIDEO_PLAY 2
#define BACK 3
#define PRE_EVENT 4
#define NEXT_EVENT 5
#define PLAY_OR_PUASE 6
#define LED_ON 7
#define LED_OFF 8
#define BEEP 9


/**
 * 定义socket相关
 */

static int fd_my_socket = -1; /* 我的邮箱地址 */

static struct sockaddr_in my_addr;  /* 我的地址 */

static struct sockaddr_in other_addr;     /* 手机的地址 */

static int addrlen  = -1; /* 地址长度 */

static int is_exit = 0;	/* 退出辅助变量 */

static pthread_t receive_thread;
static pthread_t send_camera_thread;
static pthread_t touch_thread;


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
	}

	return (void *)0;
}


/**
 * 发送摄像头数据
 * 
 * @author gec (21/01/19)
 * 
 * @return void* 
 */
static void* send_camera_thread_event()
{

	return (void *)0;
}

/**
 * 手势监控线程
 * 
 * @author gec (21/01/19)
 * 
 * @return void* 
 */
static void* touch_thread_event()
{
	int x,y,delta_x,delta_y;
	const int exit_threshold = 500;
	while (1)
	{
		scroll(&delta_x, &delta_y, &x, &y);
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
	my_addr.sin_port = htons(65000);
	my_addr.sin_addr.s_addr = inet_addr("192.168.0.10");
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
	other_addr.sin_family = AF_INET;
	other_addr.sin_port = htons(65000);
	other_addr.sin_addr.s_addr = inet_addr("192.168.0.100");
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
	int ret = pthread_create(&receive_thread, NULL, receive_thread_event, 0);
	if (!ret)
	{
		printf("create receive thread failed\n");
		return -1;
	}
	ret = pthread_create(&touch_thread, NULL, touch_thread_event, 0);
	if (!ret)
	{
		printf("create touch thread failed\n");
		return -1;
	}
	return 0;
}

static int init()
{
	/* 初始化socket */
	init_socket();
	/* 初始化线程 */
	init_threads();
	return 0;
}

static int destory()
{
	/* 关闭socket */
	if (-1 != fd_my_socket)
	{
		close(fd_my_socket);
	}
	/* 清除各个线程 */
	pthread_cancel(receive_thread);
	pthread_cancel(send_camera_thread);
	pthread_cancel(touch_thread);
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
