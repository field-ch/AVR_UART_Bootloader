#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#define F_CPU 16000000UL
#include <util/delay.h>
#include "usart.h"


/*TODO: 
*	1)	Initialize hardware
*	2)	Obtain new application code
*	3)	Erase existing application code from
*		AVR application flash section
*	4)	Write the new application code to the
*		AVR application flash section
*	5)	Jump the program counter to the new application code
*/
int main(void)
{
	cli();
	//LED test
	DDRB = 0xFF;
	PORTB = 0xFF;
	PORTB |= 0xFF;
	_delay_ms(1000);
	PORTB &= 0x00;
	_delay_ms(1000);

	
	uint32_t page = 0;	//address of the next flash page to be written to
	uint16_t word;	//next data word to write
	uint16_t i = 0;
	uint16_t filesz = 0;

	uint8_t sreg;	
	uint8_t app_code[1024];
	uint8_t * app_code_addr = app_code;	
	uint8_t rem_pages = 0;
	
	
	usart_init();	//PC serial port is connected to the UART via a converter module
	filesz = usart_rx_byte();
	filesz = filesz + (usart_rx_byte() << 8);	//receive the file size as a 16 bit number
	
	for(i = 0; i < filesz; i++){
			app_code[i] = usart_rx_byte(); //fill the buffer with the application code bytes
	}

	rem_pages = filesz/SPM_PAGESIZE;	//calculate minimum number of pages to write
	if(filesz%SPM_PAGESIZE){rem_pages++;}	//round up to the next page if there is a remainder

	while (1)
	{
		if(rem_pages){
			
			sreg = SREG;	//save the status register
			cli();	//disable interrupts
			
			eeprom_busy_wait();		//wait for any eeprom operations to finish
			boot_page_erase(page);	//erase the existing flash page
			boot_spm_busy_wait();	//wait for the memory to be erased
			
			for(i = 0; i < SPM_PAGESIZE; i+=2){	//each word is 2 bytes, so byte address must be incremented by 2
				word = *app_code_addr;	//obtain the first byte
				app_code_addr++;	//addr points to the second byte now
				word += (*app_code_addr)<<8;	//form little endian word
				app_code_addr++;	//addr points to the third byte
				
				boot_page_fill(page + i, word);	//fill the temporary page buffer with a new word
			}
			boot_page_write(page);	//write the filled temporary page buffer to the actual flash page
			boot_spm_busy_wait();	//wait for the memory to be written
			boot_rww_enable();		//enable read while write
			
			SREG = sreg;	//reset the status register
		}
		else{	//no more application code to write
			asm("jmp 0x0000");	//begin executing updated code
		}
		rem_pages--;
		page = page + SPM_PAGESIZE; //point to the next flash page
		
	}
}

