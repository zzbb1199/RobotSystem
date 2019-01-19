#include <stdio.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>
#include <strings.h>
#include <linux/input.h>

#include "lcd.h"
#include "api_v4l2.h"

void *myrecvmsg(void *arg);

FrameBuffer freambuf;

int main(int argc,char**argv)
{
	/* 打开LCD屏幕*/
	lcd_open();

	/* 初始化摄像头设备*/
	linux_v4l2_device_init("/dev/video7");

	/* 启动摄像头*/
	linux_v4l2_start_capturing();

	pthread_t tid;
	pthread_create(&tid,NULL,myrecvmsg,NULL);
	addrsize=sizeof(phoneaddr);
	udp_init();

	while(1)
	{
		// 实时监控
		/* 获取摄像头数据      存放jpg文件流*/
		linux_v4l2_get_fream(&freambuf);

		/* 显示摄像头图像*/
		lcd_draw_jpg(80, 0, NULL, freambuf.buf, freambuf.length, 0);

		sendto(iphonex,freambuf.buf,freambuf.length,0,(struct sockaddr *)&phoneaddr,sizeof(phoneaddr));
	}

	/* 停止摄像头*/
	linux_v4l2_stop_capturing();

	/* 卸载摄像头*/
	linux_v4l2_device_uinit();

	lcd_close();


	return 0;
}

void *myrecvmsg(void *arg)
{
	char buf[200];
	int addrsize=sizeof(phoneaddr);
	while(1)
	{
		usleep(1000);
		bzero(buf,200);//清零
		//聊天，收发数据，接收手机发送过来的控制命令
		recvfrom(iphonex,buf,200,0,(struct sockaddr *)&phoneaddr,&addrsize);
		printf("iphoneX send msg is:%s ip:%s  port:%hu\n",buf,inet_ntoa(phoneaddr.sin_addr),ntohs(phoneaddr.sin_port));
		//打印收到的命令
		if(strcmp(buf,"MUSIC_PLAY")==0)//字符串的比较
		{
			printf("PLAY\n");
		}

	}
}
int udp_init()
{

	int ret;
	//初始化该结构体
	bindaddr.sin_family=AF_INET;
	bindaddr.sin_addr.s_addr=inet_addr("192.168.1.99");////开发板无线网卡的ip地址
	bindaddr.sin_port=htons(2234);//端口号也需要转换

	//买手机---》创建一个udp套接字(数据报套接字)用于通信
	iphonex=socket(AF_INET,SOCK_DGRAM,0);
	if(iphonex==-1)
	{
		printf("create socket failed!\n");
		return -1;
	}
	//绑定 ---》自己ip和端口号
	ret=bind(iphonex,(struct sockaddr *)&bindaddr,sizeof(bindaddr));
	if(ret==-1)
	{
		printf("bind failed!\n");
		return -1;
	}

	return 0;
}
