/**
 * 音乐播放头文件 
 *  
 */
#ifndef __MUSIC_H__
#define __MUSIC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char * music_path[50]; /*音乐的名字*/

static int music_num = 0; /*音乐一共多少首*/

static int i = -1;	/*当前播放到第几首*/

static int isplayed = 0;	/*当前是否在播放*/

/**
 * 
 * 
 * @author raven (19-1-13)
 * 
 * @param path  播放的音乐路径
 * @param music_num 音乐数量
 * 
 * @return int 
 */
int init_music(char *path[], int music_num);

/**
 * 
 * 播放或者暂停
 * @author raven (19-1-13)
 * 
 * @return int 
 */
int pause_or_play();


static int pause_music();

static int play_music();

/**
 * 上一首音乐
 * 
 * @author raven (19-1-13)
 * 
 * @return int 
 */
int pre_music();

/**
 * 下一首音乐
 * 
 * @author raven (19-1-13)
 * 
 * @return int 
 */
int next_music();

#endif
