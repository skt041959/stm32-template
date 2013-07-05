include Makefile.common
LDFLAGS=$(COMMONFLAGS) -fno-exceptions -ffunction-sections -fdata-sections -L$(LIBDIR) -nostartfiles -Wl,--gc-sections,-Tlinker.ld

LDLIBS+=-lm
LDLIBS+=-lstm32
all: libs src
	$(CC) -o $(PROGRAM).elf $(LDFLAGS) \
		-Wl,--whole-archive \
		src/app.a \
		-Wl,--no-whole-archive \
		$(LDLIBS)
	$(OBJCOPY) -O ihex $(PROGRAM).elf $(PROGRAM).hex
	$(OBJCOPY) -O binary $(PROGRAM).elf $(PROGRAM).bin

#Extract info contained in ELF to readable text-files:
	arm-none-eabi-readelf -a $(PROGRAM).elf > $(PROGRAM).info_elf
	arm-none-eabi-size -x -t $(PROGRAM).elf > $(PROGRAM).info_size
	
#	arm-none-eabi-objdump -S $(PROGRAM).elf > $(PROGRAM).info_code

.PHONY: libs src clean flash flash_s debug emu

#libs:
	#$(MAKE) -C libs $@

src:
	$(MAKE) -C src $@

clean:
	$(MAKE) -C src $@
	#$(MAKE) -C libs $@
	- rm -f $(PROGRAM).elf $(PROGRAM).hex $(PROGRAM).bin $(PROGRAM).info_elf $(PROGRAM).info_size

#	rm -f $(PROGRAM).info_code

flash:
	do_flash main.bin

flash_s:
	st-flash write v1 main.bin 0x8000000

debug:
	arm-none-eabi-gdb --eval-command="target extended-remote localhost:3333" main.elf

emu:
	#arm-none-eabi-gdb --eval-command="target extended-remote localhost:1234" --eval-command="set $sp=0x20005000" main.elf
	arm-none-eabi-gdb --eval-command="target extended-remote localhost:1234" -x stm32.gdb main.elf

