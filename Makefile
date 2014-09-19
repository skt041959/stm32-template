include Makefile.common

SYSROOT=home/skt/code/Sourcery_CodeBench_Lite_for_ARM_EABI

LDFLAGS=$(COMMONFLAGS) -fno-exceptions -ffunction-sections -fdata-sections -L$(LIBDIR) -nostartfiles -Wl,--gc-sections,-Tstm32.ld

LDLIBS+=-lm
LDLIBS+=-lstm32
LDLIBS+=-lstm32_usb
all: libs src tags
	$(CC) -o $(PROGRAM).elf $(LDFLAGS) \
		-Wl,--whole-archive,-Map,$(PROGRAM).map \
		src/app.a \
		-Wl,--no-whole-archive \
		$(LDLIBS)
	#$(OBJCOPY) -O ihex $(PROGRAM).elf $(PROGRAM).hex
	$(OBJCOPY) -O binary $(PROGRAM).elf $(PROGRAM).bin

#Extract info contained in ELF to readable text-files:
	arm-none-eabi-readelf -a $(PROGRAM).elf > $(PROGRAM).info_elf
	#arm-none-eabi-size -x -t $(PROGRAM).elf > $(PROGRAM).info_size
	
	#arm-none-eabi-objdump -S $(PROGRAM).elf > $(PROGRAM).info_code

.PHONY: libs src clean flash flash2 flash_s debug emu

tags:$(shell find -name '*.c') $(shell find -name '*.h')
	ctags -R
	cscope -Rbqk -I $(SYSROOT)/arm-none-eabi/include

libs:
	$(MAKE) -C libs $@

src:
	$(MAKE) -C src $@

clean:
	$(MAKE) -C src $@
	#$(MAKE) -C libs $@
	- rm -f $(PROGRAM).elf $(PROGRAM).hex $(PROGRAM).bin $(PROGRAM).info_elf $(PROGRAM).info_size $(PROGRAM).map
	#- rm -f cscope.out cscope.in.out cscope.po.out
	#- rm -f tags

flash:all
	openocd -f ./flash.tcl

flash_s:
	st-flash write v1 main.bin 0x8000000

debug:flash
	arm-none-eabi-gdb main.elf --eval-command=' target remote | openocd -f interface/stlink-v2.cfg -f target/stm32f1x_stlink.cfg -c "gdb_port pipe; log_output openocd.log"' -x run_gdb.conf

emu:
	arm-none-eabi-gdb --eval-command="target extended-remote localhost:1234" -x stm32.gdb main.elf

