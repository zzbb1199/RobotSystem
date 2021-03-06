/**
 * 相册操作头文件
 */
#ifndef __ALBUM_H__
#define __ALBUM_H__

#include "lcd.h"
#include <stdio.h>


static int album_i = 0;   /*当前是第几张图片*/

static char *image_path[50]; /*相册路径*/

static int image_num = -1;  /*相片数量*/


/**
 * 初始化相册
 * 
 * @author raven (19-1-13)
 *  
 * @param addr lcd共享的内存，用于写入图片数据 
 * @param path 相片路径集合
 * @param num 相片数量
 * 
 * @return int 
 */
int init_album();

/**
 * 下一张图片
 * 
 * @author raven (19-1-13)
 * 
 * @return int 
 */
int next_image();


/**
 * 上一张图片
 * 
 * @author raven (19-1-13)
 * 
 * @return int 
 */
int pre_image();

#endif