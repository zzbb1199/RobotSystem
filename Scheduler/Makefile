CC := arm-linux-gcc
CFLAGS := -L ../lib/libjpeg -lapi_v4l2_arm -lpthread -ljpeg
INCLUDE :=-I ../mylib/ -I ../lib/libjpeg  -I../Album -I../Camera -I../CrossLine \
          -I../GuaGuaLe -I../Menu -I../MusicPlayer -I../SlideUnlock -I../Chat \
	   -I../VideoPlayer
CSources := $(wildcard *.c ../mylib/*.c )

Objs := $(CSources:.c=.o)

Scheduler:$(Objs)
	echo "compling"
	$(CC) $^ -o $@   $(CFLAGS)

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


