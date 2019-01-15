#include "touch.h"
void get_xy(int fd,int *x,int *y)
{
	struct input_event event;
	int count =0 ;
	while(1)
	{
		read(fd,&event,sizeof(event));
        if(EV_ABS == event.type)
        {
            if(ABS_X == event.code)
            {
                *x = event.value;
                count++;
            }
            else if(ABS_Y == event.code)
            {
                *y = event.value;
                count++;
            }
        }
        if(count == 2)
        {
        	return ;
        }
	}
}
