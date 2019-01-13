
#include "album.h"


int init(int *p,char *path[], int num)
{
    int i;
    for(i = 0; i < num;i++)
    {
        image_path[i] = path[i];
    }
    image_num = num;
    addr = p;
    return 0;
}

/**
 * 下一张图片
 * 
 * @author raven (19-1-13)
 * 
 * @return int 
 */
int next_image()
{
    i = (i + 1) % image_num;
    image_show(addr, image_path[i]);
    return 0;
}


/**
 * 上一张图片
 * 
 * @author raven (19-1-13)
 * 
 * @return int 
 */
int pre_image()
{
    i = (i - 1 + image_num) % image_num;
    image_show(addr, image_path[i]);
    return 0;
}
