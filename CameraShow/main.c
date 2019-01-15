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
#include "draw.h"
#include "touch.h"
#include "lcd.h"
#include "api_v4l2.h"


FrameBuffer freambuf; /* 一帧数据 */
int write_lock  = 0;    /* 写入锁 */

int fd_touch = -1;
int fd_write2file = -1;



int check_boundary(int x, int y, struct Boundary bd)
{
	if (x > bd.p1->x && y > bd.p1->y && x < bd.p2->x && y < bd.p2->y)
	{
		return 1;
	}
	return 0;
}
void* take_photo()
{
	struct Boundary take_photo_bd;
	take_photo_bd.p1 =  (struct Point *)malloc(sizeof(struct Point));
	take_photo_bd.p2 =  (struct Point *)malloc(sizeof(struct Point));

	take_photo_bd.p1->x = 720;
	take_photo_bd.p1->y = 0;

	take_photo_bd.p2->x = 800;
	take_photo_bd.p2->y = 240;

	int x, y;

	/*  写入字节数 */
	int write_ret;
	while (1)
	{
		get_xy(fd_touch, &x, &y);
		if (check_boundary(x, y, take_photo_bd))
		{
			/* 添加写入锁 */
			write_lock = 1;
			write_ret = write(fd_write2file, freambuf.buf, freambuf.length);
			printf("write_ret %d\n", write_ret);
			printf("write success!!\n");
			/* 解锁 */
			write_lock = 0;
		}

	}
}

int destory()
{
	/* 停止摄像头*/
	linux_v4l2_stop_capturing();

	/* 卸载摄像头*/
	linux_v4l2_device_uinit();

	lcd_close();

	/* 关闭触摸屏 */
	close(fd_touch);

	/* 关闭写入文件 */
	close(fd_write2file);

	return 0;
}


int init()
{
	/* 打开LCD屏幕*/
	lcd_open();

	/* 初始化摄像头设备*/
	linux_v4l2_device_init("/dev/video7");

	/* 启动摄像头*/
	linux_v4l2_start_capturing();

	/* 初始化触摸屏 */
	fd_touch = open("/dev/input/event0", O_RDONLY);

	/* 初始化需要将文件写入到的地方 */
	fd_write2file = open("./image.jpg", O_RDWR | O_CREAT, 0755);

	/* 创建拍照thread */
	pthread_t take_photo_thread;
	int ret = pthread_create(&take_photo_thread, NULL,
							 take_photo, 0);
	if (!ret)
	{
		return -1;
	}

	return 0;
}



int main(int argc, char **argv)
{


	init();


	while (1)
	{
		if (!write_lock)
		{
			/* 获取摄像头数据      存放jpg文件流*/
			linux_v4l2_get_fream(&freambuf);

			/* 显示摄像头图像*/
			lcd_draw_jpg(80, 0, NULL, freambuf.buf, freambuf.length, 0);
		}
	}

	destory();

	return 0;
}
