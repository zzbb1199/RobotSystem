#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int *addr = NULL;
int fd_lcd = -1;
int fd_touch = -1;
void init()
{
    addr = my_mmap();
    fd_lcd = open("/dev/fb0", O_RDWR);
    fd_touch = open("/dev/input/event0", O_RDONLY);
    if(-1 == fd_lcd || -1 == fd_touch)
    {
        perror("init failed");
        return;
    }
}
int destory()
{
    my_munmap(addr);
    close(fd_lcd);
    close(fd_touch);

}




int main(void)
{
	
	//创建管道
    if(access("/tmp/fifo",F_OK))
    {
        int ret = mkfifo("/tmp/fifo", 0x755);
        if(-1 == ret)
        {
            perror("create fifo failed");
            return -1;
        }
    }
	//写入信息到管道中
    int fd_fifo = open("/tmp/fifo", O_RDWR);
    if(-1 == fd_fifo)
    {
        perror("open fifo failed");
        return -1;
    }

    system("mplayer -slave -input file=/tmp/fifo -quiet -zoom -x 800 -y 480 ./1.avi &");

    int x,y;
    while(1)
    {
        get_xy(fd_touch,&x, &y);
        printf("%d,%d", x, y);
    }

    return 0;
}
