CC=avr-gcc
CC_FLAGS=-Wall -g -Os -mmcu=$(MCU)
BLS=-Wl,-section-start=.text=0x7000 #Puts the bootloader code into the default bootloader section of atmega328p (0x3800). 
#If resizing BLS or using different MCU, use .text=2*(bls start address)
MCU=atmega328p	#Change this line if using different ATMEGA
PRGMR=usbasp	#Change to appropriate programmer
BOOT=AVR_Bootloader
APP=test_blink	#just a basic blink app for testing purposes
READ=eeprom_contents #for debugging eeprom
SRC=main

all: 
	$(CC) $(CC_FLAGS) -c $(SRC).c
	$(CC) $(BLS) -mmcu=$(MCU) -o $(BOOT).elf $(SRC).o
	avr-objcopy -j .text -j .data -O ihex $(BOOT).elf $(BOOT).hex

hex2bin: #for converting the firmware hex file to a binary file prior to sending over the serial port
	avr-objcopy -I ihex $(APP).hex -O binary $(APP).bin 

fuse: #This simply changes the BOOTRST fuse to jump to the bootloader code on reset. Alter the fuses at your own risk!
	avrdude -p $(MCU) -D -c $(PRGMR) -U hfuse:w:0b11011000:m  
	
wr_eeprom:	#writes to the eeprom of the AVR
	avrdude -p $(MCU) -D -c $(PRGMR) -U eeprom:w:$(APP).hex:i -F -P usb

r_eeprom:	#reads the eeprom of the avr and stores it in a file (for debugging)
	avrdude -p $(MCU) -D -c $(PRGMR) -U eeprom:r:$(READ).hex:i -F -P usb

boot_flash:	#flash the bootloader
	avrdude -p $(MCU)  -c $(PRGMR) -U flash:w:$(BOOT).hex:i -F -P usb

app_flash:	#flash an application
	avrdude -p $(MCU) -D -c $(PRGMR) -U flash:w:$(APP).hex:i -F -P usb

r_flash:	#read the contents of the flash memory into a file (for debugging)
	avrdude -p $(MCU) -D -c $(PRGMR) -U flash:r:flashdump.txt:i -F -P usb

clean:	#remove junk files
	rm -rf $(SRC).o $(BOOT).elf $(BOOT).hex filetransfer.exe