#定义变量保存gcc,arm-linux-gcc
CC= arm-linux-gcc

ELF=WangDachui

CONFIG=-I./ -I./libjpeg -L./libjpeg -ljpeg

SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))

$(ELF):$(OBJS)
	$(CC) -o $@ $^ -lpthread $(CONFIG)

%.o:%.c
	$(CC) -c $< -o $@ $(CONFIG)

clean:
	rm *.o $(TARGET)
