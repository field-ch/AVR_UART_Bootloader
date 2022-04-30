# AVR UART Bootloader Example
## Basic Overview
This is a simple bootloader example written in C for AVR microcontrollers. When the bootloader starts up, it waits to receive the firmware update through the UART module (which is connected to the PC serial port via a converter). The PC first sends the size of the firmware update (in bytes), then the actual binary file itself. The bootloader software waits to receive the file size, after which it appropriately fills a buffer with the bytes (aka the firmware file bytes) being transmitted over the serial port. The bootloader then uses this buffer to program the flash memory of the device. 

You can use a USB to UART converter module to connect the UART of your AVR chip to a USB port of your PC. 

## How to Build
Change the variables in the makefile if you are using a different AVR chip and/or wish to resize the BLS and change the bootloader start address.

To build the bootloader, go into the command terminal and navigate to the AVR_UART_Bootloader folder. Once inside the folder, type "make" or "make all" into the terminal and hit enter. This will create the intel hex file "AVR_Bootloader.hex".

If resizing BLS, dont forget to edit the start location of the .text segment in the makefile. To set the AVR fuses, type "make fuse" into the terminal and hit enter(programmer must be connected). The fuse value in the makefile is the default BLS size for the atmega328p, the only difference is the BOOTRST fuse is set to 0 so that the reset interrupt will jump to the bootloader code. 

After adjusting the fuses, flash the bootloader hex file onto the chip by typing "make boot_flash" into the terminal and hitting enter. Reset the board to jump into the bootloader code (provided BOOTRST is programmed) and the board will be ready to receive your application code over the serial port. 

## Sending Application Files to the AVR
The C# program serial_update.cs is a very basic console app for sending the application code to the avr chip. Change it to fit your serial port specifications (ie baud rate, port name, parity, etc...). The executable expects your binary application file to be provided as a command line argument. I've provided a sample application binary test_blink.bin that simply blinks the onboard LED of the arduino uno. 

The powershell script "compile.ps1" will compile the C# file (make sure to add the C# compiler folder to your path) as well as use the executable to send test_blink.bin to the AVR. Change the application binary file name in the script to match your binary file, then simply type .\compile.ps1 and hit enter. 

If not using the compile script, compile the C# file with csc and use the resulting executable as follows: .\executable.exe application.bin




