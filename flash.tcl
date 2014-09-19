set proDir [pwd]

source [find interface/stlink-v2.cfg]
source [find target/stm32f1x_stlink.cfg]

init
#halt

reset halt

flash probe 0
stm32f1x mass_erase 0
flash write_bank 0 $proDir/main.bin 0
reset run

shutdown
