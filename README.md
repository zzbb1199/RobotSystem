## 操作说明文档

### 1. 操作说明篇

##### 1.1 环境准备

1. 在开发版上搭建程序所依赖的目录结构，具体可参见**本篇2.3小节**和提交的**代码包-开发板**。
2. 要下载可运行程序（编译参考**本篇2.1-2.2小节**）至步骤1搭建的依赖目录的根目录。
3. 安装提交的**代码包-ubuntu版**中的/lib下的所有库文件到开发版。
4. 安装madplay,mplayer,ffmpeg,alsa等所需执行文件到开发版。
5. 安装wifi,lcd驱动，插上wifi和摄像头（等开发板开机后再插入到开发板中）。
6. 一部Android手机，连接上开发板上的wifi（注意要处于统一网段），运行wifi互动程序，输入开发板ip，连接到开发板。
7. ubuntu主机操作，运行语音识别库。
8. unbuntu主机操作，进行聊天模块(chat)，运行`./Chat`命令，根据程序提示分别输入ubuntu主机的ip和开发板的ip。
9. 在开发版上，执行`./main`执行程序

##### 1.2 功能介绍

1. 滑屏解锁，两种样式，进度条方式和滑块方式(默认），可在程序菜单界面中通过"解锁方式“菜单栏更改。

2. 菜单1，菜单1共6个功能，分别为，“照相机”“音乐播放器”，“视频播放器“，”一键解锁“，”刮刮乐“，”聊天“。

3. 菜单2，菜单2共4个功能，分别为，“远程控制（wifi）”，“语音控制“，”解锁方式“，”主题选择“。

   菜单1和菜单2可通过左右滑动切换。

4. 照相机（注意插入摄像头），相机内有4个按钮

   1. 返回按钮，返回到菜单主界面
   2. 相册按钮，点击进入相册（进入相册后，可通过从左至右长滑屏幕退出到相机界面)
   3. 拍照按钮，拍照。
   4. 摄像按钮，录相按钮。

5. 音乐播放器，具有上一首，下一首，暂停/播放，返回，歌名显示的功能。

6. 视频播放器，具有：

   1. 左侧双击上一部视频，右侧双击下一部视频。
   2. 左右滑动，后退快进。
   3. 上下互动，音量加减。

7. 一键锁屏，回退到锁屏界面。

8. 刮刮乐，刮刮乐小程序。

9. 聊天，进入聊天程序后会要求输入当前主机的ip和需要通信的ip，建立连接后即可发送/接收信息。

10. 远程控制（需要wifi模块），可控制

    1. 音乐，上一首，下一首，暂停/播放,回退到远程控制界面。
    2. 视频，上一部，下一部，暂停/播放，回退到远程控制界面
    3. LED二进制加法器，通过不断点击手机上的"LED_ON"按钮，可实现二进制加法，以LED灯显示。
    4. 返回，返回到菜单2。

11. 语音控制（需要在ubuntu上开启语音识别模块），可控制

    1. 音乐，上一首，下一首，暂停/播放，回退到语音控制界面。
    2. 视频，上一部，下一步，暂停/播放，快进，后退，回退到语音控制界面。
    3. 相册，上一张，下一张，回退到语音控制界面
    4. 返回，回退到菜单2界面。

12. 锁屏方式：

    1. 滑块方式
    2. 进度条方式

13. 主题选择：

    1. 主题一
    2. 主题二

### 2. 系统环境篇

##### 2.1 代码根目录结构如下:

```
.
├── album  -		-----------------------------相册模块
├── camera 			------------------------------相机模块
├── chat   			------------------------------聊天模块
├── guaguale 		------------------------------刮刮乐模块
├── include 		------------------------------公用头文件
├── lib   			------------------------------库文件
├── Makefile  		------------------------------项目Makefile
├── menu      		------------------------------菜单模块
├── music_player 	------------------------------音乐播放器模块
├── remote_control  ------------------------------远程控制模块（wifi)
├── scheduler.c     ------------------------------程序调度器（主程序源文件)
├── slide_unlock    ------------------------------滑动解锁模块
├── source		    ------------------------------公用公用源文件
├──theme			------------------------------主题切换模块
├── video_player	------------------------------视频播放器模块
└── voice_recon		------------------------------语音识别模块


```

##### 2.2 系统编译方法

在源代码目录下使用`make`命令即可，清除使用`make clean`命令（若提示删除文件不存在可忽略）。

生成可执行文件 **main**,下载到开发板即可。

##### 2.3 开发板根目录结构

```
.
├── Image			------------------------------相册图片
├── lib				------------------------------部分依赖库
├── main
├── Music			------------------------------音乐文件
├── Theme1			------------------------------主题一的图片文件
├── Theme2			------------------------------主题二的图片文件
└── Video			------------------------------视频存放地
    ├── Image		------------------------------相机录屏产生的中间图像文件

```

**使用方式：** 将编译好的程序下载到开发版上并放置在目录结构的根目录（注意不是linux系统的根目录），执行`./main`即可。




