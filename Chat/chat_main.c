#include "socket.h"
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include "chat_main.h"


static int fd_my_socket = -1; /* 我的邮箱地址 */

static struct sockaddr_in my_addr;  /* 我的地址 */

static struct sockaddr_in other_addr;     /* 其他人群的地址 */

static int addrlen  = -1; /* 地址长度 */

/**
 * 发送一次信息
 * 
 * @author gec (16/01/19)
 *  

 *  
 * @param msg  发送的信息
 * 
 * @return int 返回发送的字节数,错误则返回-1
 */
static int my_send(char *msg);


/**
 * 接受一次信息
 * 
 * @author gec (16/01/19)
 * 
 * @param msg 

 * 
 * @return int 
 */
static int my_receive(char *msg,int len);



/**
 * 发送线程指针
 * 
 * @author gec (16/01/19)
 * 
 * @return void* 
 */
static void* sending_thread_event();

/**
 * 接受线程指针
 * 
 * @author gec (16/01/19)
 * 
 * @return void* 
 */
static void* receiving_thread_event();



/**
 * 程序初始化
 * 
 * @author gec (16/01/19)
 * 
 * @return int 
 */
static int init();

/**
 * 初始化sockets
 */
static int init_sockets();

/**
 * 初始化县城
 * 
 * @author gec (16/01/19)
 * 
 * @return int 
 */
static int init_threads();

/**
 * 销毁资源
 * 
 * @author gec (16/01/19)
 * 
 * @return int 
 */
static int destory();

int chat_main(void)
{
	/* 初始化 */
	init();
	/* 销毁资源 */
	destory();
	while (1);
	return 0;
}


/***************************初始化 start**********************************************/
static int init()
{
	addrlen = sizeof(struct  sockaddr_in);
	/* 初始化sockets */
	init_sockets();
	/* 初始化线程 */
	init_threads();
	return 0;
}
/***************************初始化 end**********************************************/




/*********************************发送接受线程 start****************************************/
static void* sending_thread_event()
{
	printf("===start sending thread====\n");
	char msg[100];
	while (1)
	{
		fgets(msg, sizeof(msg), stdin);
		my_send(msg);
	}
	return (void *)0;
}

static void* receiving_thread_event()
{
	printf("======start reciving thread====\n");
	char msg[100];
	int ret = -1;
	while (1)
	{
		ret = my_receive(msg,sizeof(msg));
		if (-1 == ret)
		{
			continue;
		}
		/* 输出msg */
		printf("%s\n", msg);
	}
	return (void *)0;
}

/********************************发送接受线程 end*****************************************/


/****************************发送接受 start*********************************************/
static int my_send(char *msg)
{
	printf("send msg:%s", msg);
	int ret = -1;
	ret =  sendto(fd_my_socket, msg,
				  strlen(msg),
				  0, (struct sockaddr *)&other_addr,
				  addrlen);

	if (-1 == ret)
	{
		perror("send error");
		return -1;
	}
	printf("send bytes %d\n", ret);
	return ret;
}

static int my_receive(char *msg,int msg_len)
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
/******************************发送接受 end*******************************************/

static int init_sockets()
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

	char ip[100];
	printf("input owner's ip:\n");
	scanf("%s", ip);

	/* 绑定地址 */
	/* 初始化自己的地址 */
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(65000);
	my_addr.sin_addr.s_addr = inet_addr(ip);
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

	bzero(ip, strlen(ip));
	printf("input other's ip:\n");
	scanf("%s", ip);
	/* 初始化别人的地址 */
	other_addr.sin_family = AF_INET;
	other_addr.sin_port = htons(65000);
	other_addr.sin_addr.s_addr = inet_addr(ip);
}

static int init_threads()
{
	/* 创建触摸屏事件thread */
	pthread_t sending_thread, reciving_thread;

	int ret1 = pthread_create(&sending_thread, NULL,
							  sending_thread_event, 0);
	
	int ret2 = pthread_create(&reciving_thread, NULL,
							  receiving_thread_event, 0);

	if (!ret1 || !ret2)
	{
		return -1;
	}
	return 0;
}

static int destory()
{
//	close(fd_my_socket);
	return 0;
}
