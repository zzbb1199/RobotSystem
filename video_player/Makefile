CC := arm-linux-gcc
CFLAGS := -L ../lib/libjpeg -lapi_v4l2_arm -lpthread -ljpeg
INCLUDE :=-I ../mylib/ -I ../lib/libjpeg 
CSources := $(wildcard *.c ../mylib/*.c )

Objs := $(CSources:.c=.o)


VideoPlayer:$(Objs)
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


