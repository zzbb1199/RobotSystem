#ifndef __LCD_H__
#define __LCD_H__

#define LCD_WIDTH  			800
#define LCD_HEIGHT 			480
#define FB_SIZE				(LCD_WIDTH * LCD_HEIGHT * 4)

#define EN_LCD_SHOW_JPG		1

#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int iphonex;
struct sockaddr_in phoneaddr ;
struct sockaddr_in bindaddr;
int addrsize ;

#endif