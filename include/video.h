/**
 * 视频播放器头文件
 */

#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char *video_path[50]; /*视频文件路径*/

static int video_num = -1;	/*视频数量*/
static int i = 0;   /*当前视频index*/

static int fd_fifo = -1;    /*视频控制管道文件*/



/**
 * 视频播放器初始化
 * 
 * @author gec (14/01/19)
 * 
 * @param path 视频文件路径
 * @param num 视频文件数量
 * @param fd_fifo 
 *  			  控制播放器所用到的管道文件描述符
 *  
 *  
 * @return int 
 */
int init_video(int fd_fifo);
/**
 * 播放或暂停
 * 
 * @author gec (14/01/19)
 * 
 * @return int 
 */
int play_or_pause();

/**
 * 快进视频播放
 * 
 * @author gec (14/01/19)
 * 
 * @param sec 快进秒数
 * 
 * @return int 
 */
int forward(int sec);

/**
 * 后退视频
 * 
 * @author gec (14/01/19)
 * 
 * @param sec 后退秒数
 * 
 * @return int 
 */
int backward(int sec);

/**
 * 音量加
 * 
 * @author gec (14/01/19)
 * 
 * @param level 音量加多少
 * 
 * @return int 
 */
extern int volume_up(float level);

/**
 * 音量减少
 * 
 * @author gec (14/01/19)
 * 
 * @param level  音量减少多少
 * 
 * @return int 
 */
extern int volume_down(float level);


/**
 * 前一个
 * 
 * @author gec (14/01/19)
 * 
 * @return int 
 */
int pre_video();


/**
 * 下一个
 * 
 * @author gec (14/01/19)
 * 
 * @return int 
 */
int next_video();

/**
 * 销毁视频
 * 
 * @author gec (21/01/19)
 * 
 * @return int 
 */
int destory_video();
#endif


