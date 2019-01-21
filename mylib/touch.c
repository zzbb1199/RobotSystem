#include "touch.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int fd_touch = -1;   /* 触摸板资源 */

int touch_open()
{
	fd_touch = open("/dev/input/event0", O_RDONLY);
	if (-1 == fd_touch)
	{
		perror("open touch failed\n");
		return -1;
	}
	else
	{
		printf("open touch sucess\n");
	}
	return 0;
}

int touch_close()
{
	close(fd_touch);
}

void get_xy(int *x, int *y)
{
	struct input_event event;
	int count = 0;
	while (1)
	{
		read(fd_touch, &event, sizeof(event));
		if (EV_ABS == event.type)
		{
			if (ABS_X == event.code)
			{
				*x = event.value;
				count++;
			}
			else if (ABS_Y == event.code)
			{
				*y = event.value;
				count++;
			}
		}
		if (count == 2)
		{
			return;
		}
	}
}


/**
 * 
 * 
 * @author gec (16/01/19)
 * 
 * @param x 
 * @param y 
 * @param bd 
 * 
 * @return int 
 */
int check_boundary(int x, int y, struct Boundary bd)
{
	if (x > bd.p1->x && y > bd.p1->y && x < bd.p2->x && y < bd.p2->y)
	{
		return 1;
	}
	return 0;
}


int scroll(int *delta_x, int *delta_y,int *x, int *y)
{
	struct input_event event;
	int count = 0;;
	int start_x, start_y, end_x, end_y;
	*x = -1;
	*y = -1;
	*delta_x = 0;
	*delta_y = 0;
	while (1)
	{
		read(fd_touch, &event, sizeof(event));
		//printf("====type :%d,code :%d  value:%d====\n", event.type, event.code,event.value);
		if (EV_ABS == event.type)
		{
			if (ABS_X == event.code)
			{
				count = 0;
				//printf("set x\n");
				*x = event.value;
				count++;
			}
			else if (ABS_Y == event.code)
			{
				//printf("set y\n");
				*y = event.value;
				count++;
			}
		}


		if (EV_KEY == event.type)
		{
			//printf("press event\n");
			if (BTN_TOUCH == event.code && 1 == event.value)
			{
					//printf("set start\n");
					start_x = *x;
					start_y = *y;
			}
			else if (BTN_TOUCH == event.code && 0 == event.value)
			{
					//printf("set end\n");
					end_x = *x;
					end_y = *y;
					//一次滑动完成
					*delta_x = start_x - end_x;
					*delta_y = start_y - end_y;
					//printf("delta x=%d,delta_y = %d\n", *delta_x, *delta_y);
				return 0;
			}
		}

		if (2 == count)
		{
			count = 0;
		}
	}
	return 0;
}

int free_boundary(struct Boundary *bd)
{
	if (bd->p1)
	{
		free(bd->p1);
	}
	if (bd->p2)
	{
		free(bd->p2);
	}
	return 0;
}
