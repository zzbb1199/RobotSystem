/**
 * 摄像机主程序
 */
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>
#include <strings.h>
#include <stdlib.h>
#include <linux/input.h>
#include "touch.h"
#include "lcd.h"
#include "api_v4l2.h"
#include "public_resource.h"
#include "scheduler.h"

static FrameBuffer freambuf; /* 一帧数据 */

#define NO_TOUCH_BLOCK 0
#define TOUCH_BLOCK 1
static volatile int touch_block; /* 添加触摸屏阻塞锁 */

static unsigned int record_touch_times = 0; /* 录屏点击次数 */

pthread_t touch_event_thread; /* 监控触摸屏县城 */

pthread_t thread_image2video; /* 视频转换线程 */

static int isconvert_video;

static int touch_type;  /* 点击的类型 */



/**
 * 点击事件定义
 */
#define NO_TOUCH_EVENT 0
#define TAKE_PHOTO_EVENT 1
#define RECORD_VIDEO_EVENT 2
#define BACK_EVENT 3
#define ALBUM_EVENT 4





void* image2video(void *num)
{
	isconvert_video = 1;
	int *record_video_num = (int *)num;
	printf("==========convert from image to video start===============\n");
	char cmd[50];
	sprintf(cmd, "%s ./Video/video%d.avi", "ffmpeg -f image2 -i ./Video/Image/image%d.jpg"
			, *(record_video_num)++);
	system(cmd);
	printf("==========convert from image to video end===============\n");
	return (void *)0;
}

void* touch_event()
{
	/* 初始化点击拍照边界 */
	struct Boundary take_photo_bd;
	take_photo_bd.p1 = (struct Point *)malloc(sizeof(struct Point));
	take_photo_bd.p2 = (struct Point *)malloc(sizeof(struct Point));
	take_photo_bd.p1->x = 720;
	take_photo_bd.p1->y = 0;
	take_photo_bd.p2->x = 800;
	take_photo_bd.p2->y = 240;

	/* 初始化录像拍照边界 */
	struct Boundary record_video_bd;
	record_video_bd.p1 = (struct Point *)malloc(sizeof(struct Point));
	record_video_bd.p2 = (struct Point *)malloc(sizeof(struct Point));
	record_video_bd.p1->x = 0;
	record_video_bd.p1->y = 240;
	record_video_bd.p2->x = 80;
	record_video_bd.p2->y = 480;

	struct Boundary back_bd;
	back_bd.p1 = (struct Point *)malloc(sizeof(struct Point));
	back_bd.p2 = (struct Point *)malloc(sizeof(struct Point));
	back_bd.p1->x = 0;
	back_bd.p1->y = 0;
	back_bd.p2->x = 80;
	back_bd.p2->y = 240;

	struct Boundary album_bd;
	album_bd.p1 = (struct Point *)malloc(sizeof(struct Point));
	album_bd.p2 = (struct Point *)malloc(sizeof(struct Point));
	album_bd.p1->x = 720;
	album_bd.p1->y = 240;
	album_bd.p2->x = 800;
	album_bd.p2->y = 480;

	/* 保存触摸点 */
	int x, y;

	while (1)
	{
		get_xy(&x, &y);
		printf("%d,%d\n", x, y);
		/* 检查是否触摸在拍照区域 */
		if (!touch_block && check_boundary(x, y, take_photo_bd))
		{
			printf("take photo!!!!\n");
			touch_type = TAKE_PHOTO_EVENT;
			touch_block = TOUCH_BLOCK;
			while (touch_block);  /* 启动触摸锁 */
			/* 检查是否触摸在录屏区域 */
		}
		else if (!touch_block && check_boundary(x, y, record_video_bd))
		{
			printf("record video\n");
			touch_type = RECORD_VIDEO_EVENT;
			record_touch_times++;
		}
		else if (!touch_block && check_boundary(x, y, back_bd))
		{
			printf("back event \n");
			touch_type = BACK_EVENT;
		}
		else if (!touch_block && check_boundary(x, y, album_bd))
		{
			printf("album event\n");
			touch_type = ALBUM_EVENT;
		}
	}

	return (void *)0;
}

static int destory()
{
	/* 停止摄像头*/
	linux_v4l2_stop_capturing();

	/* 卸载摄像头*/
	linux_v4l2_device_uinit();

	/* 关闭lcd */
	lcd_close();

	/* 关闭touch */
	touch_close();

	/* 关闭触摸屏监控线程 */
	pthread_cancel(touch_event_thread);

	if (isconvert_video)
	{
		isconvert_video = 0;
		/* 关闭转换县城 */
		pthread_cancel(thread_image2video);
	}

	return 0;
}


static int init()
{
	/* 打开LCD屏幕*/
	lcd_open();

	/* 打开触摸屏 */
	touch_open();

	/* 初始化摄像头设备*/
	linux_v4l2_device_init("/dev/video7");

	/* 启动摄像头*/
	linux_v4l2_start_capturing();

	/* 初始化触摸屏 */
	fd_touch = open("/dev/input/event0", O_RDONLY);

	/* 清理多余视频 */
	system("rm ./Video/video?.avi");
	system("rm  ./Video/Image/*");

	/* 初始化点击事件 */	
	touch_type = 0;
	touch_block = 0;

	/* 初始化转换视频标志位 */
	isconvert_video = 0;

	/* 初始化帧 */
	bzero(freambuf.buf, sizeof(freambuf));
	freambuf.length = 0;

	/* 创建触摸屏事件thread */
	
	int ret = pthread_create(&touch_event_thread, NULL,
							 touch_event, 0);
	if (!ret)
	{
		return -1;
	}

	return 0;
}



int camera_main(int *condition)
{
	/* 初始化程序资源 */
	init();

	/*  写入字节数临时变量 */
	int write_ret;
	/* 定义写入文件描述符 */
	int fd_write2file = -1;
	/* 拍照时,第几张照片 */
	int take_photo_num = 1;
	/* 录屏时第几帧图片 */
	int record_image_num = 0;
	/* 录屏的第几个视频 */
	int record_video_num = 1;
	
	

	/* 图片名字 */
	char image_name[50];
	bzero(image_name, 50);

	while (1)
	{

		/* 获取摄像头数据      存放jpg文件流*/
		linux_v4l2_get_fream(&freambuf);

		/* 显示摄像头图像*/
		lcd_draw_jpg(80, 0, NULL, freambuf.buf, freambuf.length, 0);

		if (TAKE_PHOTO_EVENT == touch_type)
		{

			/* 初始化需要将文件写入到的地方 */
			sprintf(image_name, "./Image/image%d.jpg", take_photo_num);
			fd_write2file = open(image_name, O_RDWR | O_CREAT | O_TRUNC, 0755);
			if (-1 == fd_write2file)
			{
				perror("open image file dec error!");
				return 0;
			}

			/* 写入 */
			write_ret = write(fd_write2file, freambuf.buf, freambuf.length);
			printf("write_ret %d\n", write_ret);
			take_photo_num++;
			/* 回收资源 */
			close(fd_write2file);

			/* 解开触摸锁 */
			touch_block = NO_TOUCH_BLOCK;
			/* 解开点击事件 */
			touch_type = NO_TOUCH_EVENT;
		}
		else if (RECORD_VIDEO_EVENT == touch_type)
		{

			/* 初始化照片写入地方 */
			sprintf(image_name, "./Video/Image/image%d.jpg", record_image_num);
			fd_write2file = open(image_name, O_RDWR | O_CREAT | O_TRUNC, 0755);
			if (-1 == fd_write2file)
			{
				perror("open image file dec error!");
				return -1;
			}

			/* 写入 */
			write_ret = write(fd_write2file, freambuf.buf, freambuf.length);
			printf("write_ret %d\n", write_ret);
			/* 回收资源 */
			close(fd_write2file);

			record_image_num++;

			if (2 == record_touch_times)
			{
				record_touch_times = 0;
				record_image_num = 0;
				/* 开启转视频的线程 */
			
				int create_thread_ret = pthread_create(&thread_image2video,
													   NULL, image2video,
													   &record_video_num);
				if (!create_thread_ret)
				{
					perror("create image to video thread error!");
				}
				/* 释放点击锁 */
				touch_block = NO_TOUCH_BLOCK;
				/* 解开点击事件 */
				touch_type = NO_TOUCH_EVENT;
			}
		}
		else if (BACK_EVENT == touch_type)
		{
			*condition = MENU;
			break;
		}
		else if (ALBUM_EVENT == touch_type)
		{
			*condition = ALBUM;
			break;
		}

	}
	destory();

	return 0;
}

