include ../Makefile.common
LIBS+=libstm32.a
CFLAGS+=-c
all: libs
libs: $(LIBS)

libstm32.a:
	@echo -n "Building $@ ..."
	cd ./CoreSupport && \
		$(CC) $(CFLAGS) \
		core_cm3.c
	cd ./src && \
		$(CC) $(CFLAGS) \
		-D"assert_param(expr)=((void)0)" \
		-I../CoreSupport\
		-I../inc \
		*.c
	$(AR) cr $(LIBDIR)/$@ \
		./CoreSupport/core_cm3.o \
		./src/*.o
	@echo "done."

.PHONY: libs clean

clean:
	-rm -f $(STMLIB)/STM32F10x_StdPeriph_Driver/src/*.o
	-rm -f $(STMLIB)/CMSIS/CM3/CoreSupport/core_cm3.o
	-rm -f $(LIBS)

