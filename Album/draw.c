#include "draw.h"


void draw_bg(int *addr,int color)
{
    //write
    int row,col;
    for(row = 0;row<HEIGHT;row++)
    {
        for(col = 0;col<WIDTH;col++)
        {
            *(addr+row*WIDTH+col) = color;
        }
    }
}


/**
 * 输出图片
 */
int image_show(int *addr,char *image_path)
{
    draw_bg(addr,0x000000);
	// check wether lcd_addr is init or not
	if(!addr)
	{

		printf("%s\n","please initialize lcd_addr" );
		return -1;
	}

    //open image
    int fd_image = open(image_path,O_RDONLY);
    if(-1 == fd_image)
    {
        perror("open image failed");
        return -1;
    }

    //处理offset
    //lseek(fd_image,54,SEEK_SET);
    char image_info[54];
    read(fd_image,image_info,54);

    printf("=========this image'info========\n");
    printf("image path:%s\n",image_path);
    int width = image_info[19]<<8 | image_info[18];
    int height = image_info[23]<<8 | image_info[22];
    printf("widht=%d\n",width);
    printf("height=%d\n",height);
    printf("width=%x\n",width);
    printf("height=%x\n",height);

    char buffer[width*height*3];
    int ret = read(fd_image,buffer,width*height*3);

    if(width > WIDTH|| height > HEIGHT)
    {
        printf("this image is too large, please change a image and try again!\n'");
        return -1;
    }

    //wait for optimize
    int start_x,start_y;
    int end_x,end_y;
    start_x = WIDTH/2-width/2;
    start_y = HEIGHT/2-height/2;
    end_x = WIDTH/2+width/2;
    end_y = HEIGHT/2+height/2;

    //output
    int x,y;
    int color_index;
    for(y = start_y;y<end_y;y++)
    {
        for(x = start_x;x<end_x;x++)
        {
            //printf("(%d,%d)\n",x,y);
            color_index = (y-start_y)*width+x-start_x;
            *(addr+(HEIGHT-1-y)*WIDTH+x) = buffer[color_index*3 + 0] <<0 |
                buffer[color_index*3 + 1] <<8 |
                buffer[color_index*3 + 2] <<16;
        }
    }

    printf("\n");

    close(fd_image);
    return 0;
}

/**
 * 比较a,b返回最小的那个
 * 
 * @author gec (14/01/19)
 * 
 * @param a 
 * @param b 
 * 
 * @return int 
 */
static int min(int a, int b)
{
    return a < b ? a : b;
}

void draw_cross_line(int *addr,struct Line line)
{
    //绘制第一条线 横轴
    draw_bg(addr,0x000000);
    int start_x = 0;
    int start_y = line.y;
    int end_x = WIDTH;
    int end_y = min(line.y+10,HEIGHT);

    int line_color = 0xffffff;
    int x,y;
    for(x = start_x;x<end_x;x++)
    {
        for(y = start_y;y<end_y;y++)
        {
            *(addr+y*WIDTH+x) = line_color;
        }
    }
    //第二条
    start_x = line.x;
    start_y = 0;
    end_x = min(line.x + 10, WIDTH);
    end_y = HEIGHT;
    for(x = start_x;x<end_x;x++)
    {
        for(y = start_y;y<end_y;y++)
        {
            *(addr+y*WIDTH+x) = line_color;
        }
    }
}
