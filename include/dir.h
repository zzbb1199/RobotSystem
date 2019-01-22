/**
 * dir.h 
 * 文件：包含对目录的读取，目录下的文件名读取 
 */

#ifndef __DIR_H__
#define __DIR_H__

#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

/**
 * 
 * 
 * @author raven (19-1-13)
 * 
 * @param dir_path 需要读取的目录路径
 * @param type 需要读取的文件类型
 * @param save_path 将读取到的文件保存在save_path中
 * @param num 一共读取到多少个文件
 * 
 * @return int 
 */
int read_files(char *dir_path, char *type
			   , char *save_path[], int *num);

 #endif
