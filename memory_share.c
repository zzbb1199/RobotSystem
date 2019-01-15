#include "memory_share.h"

int*  my_mmap()
{
	int fd_lcd = open("/dev/fb0",O_RDWR);
	if(-1 == fd_lcd)
	{
		perror("open lcd failed!");
		return NULL;
	}
     int *addr = (int *)mmap(NULL,SIZE,PROT_WRITE|PROT_READ,MAP_SHARED,fd_lcd,0);
    if(MAP_FAILED == addr)
    {
        perror("apply memo failed");
        return NULL; 
    }
    return addr;
}


int my_munmap(int *addr)
{
    int ret = munmap(addr,SIZE);
    return 0;
}
