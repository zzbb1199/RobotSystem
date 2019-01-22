CC := arm-linux-gcc

# 库文件存放地址
CFLAGS := -L ./lib
# 链接库
CFLAGS += -lxml2 \
	-ljpeg \
	-lasound \
	-lz \
	-lvnet \
	-lapi_v4l2_arm \
	-lpthread \
	-lfont
# 头文件所在地址
INCLUDE :=-I ./include/   \
	-I ./include/alsa \
	-I ./include/alsa/sound \
	-I ./include/libxml \
	-I ./album \
	-I ./camera \
	-I ./chat \
	-I ./cross_line \
	-I ./guaguale \
	-I ./menu \
	-I ./music_player \
	-I ./remote_control \
	-I ./scheduler \
	-I ./slide_unlock \
	-I ./video_player \
	-I ./voice_recon


CSources := $(shell find -name "*.c")

Objs := $(CSources:.c=.o)

LIB_JPEG :=$(wildcard ./lib/libjpeg*)

main :$(Objs)
	$(CC) $(LIB_JPEG)  $^ -o $@   $(CFLAGS)

%.d : %.c
	rm -f $@; \
	$(CC) -MM $(INCLUDE) $< > $@.$$$$; \
	sed -e 's,^.*:,$*.o $@:,g'  < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o : %.c
	$(CC) -c $(INCLUDE) $< -o $@

.PHONY : clean

clean:
	@echo $(CSources)
	rm main $(CSources:.c=.d) $(CSources:.c=.o)


