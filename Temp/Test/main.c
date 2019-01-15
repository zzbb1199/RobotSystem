#include <stdio.h>
#include "touch.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int fd_touch =  -1;

int init()
{
    fd_touch = open("/dev/input/event0",O_RDONLY);
    return 0;
}

int main(void)
{
    int x,y;
    while(1)
    {
       // get_xy(fd_touch,&x,&y);
        printf("%d,%d",x,y);
    }
    return 0;
}
