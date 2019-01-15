#定义变量保存gcc,arm-linux-gcc
CC=arm-linux-gcc

ELF=Camera

CONFIG=-I./ -I./libjpeg -L./libjpeg -ljpeg -lapi_v4l2_arm

SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))

$(ELF):$(OBJS)
	$(CC) -o $@ $^ -lpthread $(CONFIG)

%.o:%.c
	$(CC) -c $< -o $@ $(CONFIG)

clean:
	rm *.o $(TARGET)
