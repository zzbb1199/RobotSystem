CC := arm-linux-gcc
CFLAGS := -L ../lib/libjpeg \
	 -L ../lib/libvoice \
	 -L ../lib/libfont
CFLAGS += -lxml2 \
	-ljpeg \
	-lasound \
	-lz \
	-lvnet \
	-lapi_v4l2_arm \
	-lpthread 	\
	-lfont
INCLUDE :=-I ../mylib/   \
	-I ../lib/libjpeg \
	 -I ./           \
	 -I../lib/incvoice \
	 -I ../lib/incvoice/libxml \
	 -I ../lib/incfont

VOICE_LIB = #$(wildcard ../lib/voice_lib/*)

CSources := $(wildcard *.c ../mylib/*.c)

Objs := $(CSources:.c=.o)

Main:$(Objs)
	$(CC) $(VOICE_LIB) $^ -o $@   $(CFLAGS)

-include $(CSources:.c=.d)


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
	rm Main $(CSources:.c=.d) $(CSources:.c=.o)


