#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	
	//创建管道
    int ret = mkfifo("/tmp/named_pipe", 0x755);
    if(-1 == ret)
    {
        perror("create fifo failed");
        return -1;
    }
	//写入信息到管道中
    int fd_fifo = open("/tmp/named_pipe", O_RDWR);
    if(-1 == fd_fifo)
    {
        perror("open fifo failed");
        return -1;
    }

    char buf[80];
    while(1)
    {
        bzero(buf, 80);
        fgets(buf, 80, stdin);
        write(fd_fifo, buf, strlen(buf));
    }
    return 0;
}
