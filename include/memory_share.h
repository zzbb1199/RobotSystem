#ifndef __MEMORY_SHARE__
#define __MEMORY_SHARE__
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


/**
 * 内存映射
 */
int* my_mmap();

int my_munmap(int *addr);

#endif
