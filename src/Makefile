include ../Makefile.common

OBJS+=startup.o
OBJS+=main.o
OBJS+=newlib_stubs.o
OBJS+=stm32f10x_delay.o

all: src

src: app.a

app.a: $(OBJS)
	$(AR) cr app.a $(OBJS)

#startup.o:startup.s
#	$(CC) $(CFLAGS) -c -o startup.o startup.s

.PHONY: src clean

clean:
	rm -f app.a $(OBJS)

