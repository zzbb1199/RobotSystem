#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>
#include <pthread.h>

#include "lcd.h"

int main(int argc,char **argv)
{
	//打开lcd屏
	lcd_open();
	
	//显示1.jpg文件,x=0,y=0,正常大小显示
	lcd_draw_jpg(0,0,"./Image/1.jpg",NULL,0,0);
	//lcd_draw_jpg(0,0,NULL,dest,ret,0);
	
	
	//关闭lcd屏
	lcd_close();


	
	return 0;	
}

