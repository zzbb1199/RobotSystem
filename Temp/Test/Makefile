CC := gcc
CFLAGS :=
INCLUDE := -I ../
CSources := $(wildcard *.c ../*.c )

Objs := $(CSources:.c=.o)

main:$(Objs)
	echo "compling"
	$(CC)  $(CFLAGS) -o $@ $^

-include $(CSources:.c=.d)


%.d : %.c
	rm -f $@; \
	$(CC) -MM $(INCLUDE) $< > $@.$$$$; \
	sed -e 's,^.*:,$*.o $@:,g'  < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

.PHONY : clean

clean:
	@echo $(CSources)
	rm main $(CSources:.c=.d) $(CSources:.c=.o)

