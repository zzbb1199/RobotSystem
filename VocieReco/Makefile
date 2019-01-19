CC := arm-linux-gcc
CFLAGS := -L ../lib/libjpeg -L ../lib -L ../lib/voice \
 -lapi_v4l2_arm -lpthread -ljpeg -lxml2 -lasound -lz -lvnet 
INCLUDE :=-I ../mylib/ -I ../lib/libjpeg -I../lib/voice/libxml -I ./ \
 -I../Scheduler  

CSources := $(wildcard *.c ../mylib/*.c )

Objs := $(CSources:.c=.o)
Objs += $(wildcard ../lib/voice/*so*)


VoiceRecon:$(Objs)
	echo "compling"
	$(CC) $^ -o $@   $(CFLAGS) -Wl,-rpath=. -Wl,-rpath=../lib/voice -Wall

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
	rm main $(CSources:.c=.d) $(CSources:.c=.o)


