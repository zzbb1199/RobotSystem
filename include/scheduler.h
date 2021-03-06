#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

extern int condition;	/* 功能调度 */
extern int is_progress_bar; /* 解锁样式选择 */
extern int theme_choose;	/* 主题选择 */
/**
 * 功能宏定义
 */
#define LOCK 0X01
#define MENU 0x02
#define MUSIC_PLAYER 0X03
#define VIDEO_PLAYER 0X04
#define ALBUM 0x05
#define MORE  0x06
#define GUAGUA 0x07
#define CROSS_LINE 0x08
#define CAMERA 0x09
#define MENU2 0x0a
#define REMOTE_CONTROL 0x0b
#define CHAT 0x0c
#define VOICE_RECON 0x0d
#define LOCK_STYLE 0X0e
#define THEME_CHOOSE 0x0f

#define THEME1 1
#define THEME2 2
///**
// * 退出到上一级别功能
// *
// * @author gec (17/01/19)
// *
// * @return int
// */
//int back_fun();
//
//
///**
// *
// * 走到下一个功能
// * @author gec (17/01/19)
// *
// * @param fun
// *
// * @return int
// */
//int goto_fun(int fun);



#endif
