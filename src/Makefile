include ../Makefile.common

ifeq ($(CHIP),HD)
	OBJS+=startup/startup_stm32f10x_hd.o
endif
ifeq ($(CHIP),MD)
	OBJS+=startup/startup_stm32f10x_md.o
endif
ifeq ($(CHIP),LD)
	OBJS+=startup/startup_stm32f10x_ld.o
endif
OBJS+=./main.o
OBJS+=./newlib_stubs.o
OBJS+=./system_stm32f10x.o
OBJS+=./hw_config.o
OBJS+=./stm32_it.o

OBJS+=./usb_desc.o \
	  ./usb_endp.o \
	  ./usb_istr.o \
	  ./usb_prop.o \
	  ./usb_pwr.o

all: src

src: app.a

app.a: $(OBJS)
	$(AR) cr app.a $(OBJS)

#startup.o:startup.s
#	$(CC) $(CFLAGS) -c -o startup.o startup.s

.PHONY: src clean

clean:
	-rm -f app.a *.o
	-rm -f startup/*.o

