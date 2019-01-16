#定义变量保存gcc,arm-linux-gcc
CC=arm-linux-gcc

ELF=cam

CONFIG=-I ./libjpeg -L ./libjpeg -lapi_v4l2_arm -lpthread -ljpeg

SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))

$(ELF):$(OBJS)
	$(CC) -o $@ $^ -lpthread $(CONFIG)

%.o:%.c
	$(CC) -c $< -o $@ $(CONFIG)

clean:
	rm *.o $(TARGET)
