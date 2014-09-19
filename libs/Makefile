include ../Makefile.common
LIBS+=libstm32.a libstm32_usb.a
CFLAGS+=-c
all: libs
libs: $(LIBS)

libstm32.a:
	cd ./CoreSupport && \
		$(CC) $(CFLAGS) \
		core_cm3.c
	cd ./STM32F10x_StdPeriph_Driver/src && \
		$(CC) $(CFLAGS) \
		-D"assert_param(expr)=((void)0)" \
		-I../../CoreSupport\
		*.c
	$(AR) cr $(LIBDIR)/$@ \
		./CoreSupport/core_cm3.o \
		./STM32F10x_StdPeriph_Driver/src/*.o

libstm32_usb.a:
	cd ./STM32_USB-FS-Device_Driver/src && \
		$(CC) $(CFLAGS) \
		-D"assert_param(expr)=((void)0)" \
		-I../../CoreSupport\
		*.c
	$(AR) cr $(LIBDIR)/$@ \
		./STM32_USB-FS-Device_Driver/src/*.o

.PHONY: libs clean

clean:
	-rm -f STM32F10x_StdPeriph_Driver/src/*.o
	-rm -f STM32_USB-FS-Device_Driver/src/*.o
	-rm -f CoreSupport/core_cm3.o
	-rm -f $(LIBS)

