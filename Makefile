target = cfp
cflags = -g -O2
ldflags = -lm -lgc
cc = gcc

src = $(shell find src/ -name '*.c' -not -name 'main.c')
obj = $(src:.c=.o)

all: $(target)

$(target): $(obj) src/main.o
	gcc $(cflags) $(ldflags) -o $@ $^

$(obj):%.o:%.c
	$(cc) -c $(cflags) $< -MD -MF $@.d -o $@

src/main.o:src/main.c
	gcc -c $(cflags) $< -MD -MF $@.d -o $@

clean:
	-rm $(shell find . -name '*.o' -or -name '*.d')
	-rm $(target)

DEPS := $(shell find . -name *.d)
ifneq ($(DEPS),)
include $(DEPS)
endif
