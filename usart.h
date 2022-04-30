#ifndef _USART_H
#define _USART_H

#include <avr/io.h>

#define BAUD 9600UL
#define BRRV (((F_CPU/(16*BAUD)))-1)

void usart_init();
void usart_tx_byte(uint8_t);
uint8_t usart_rx_byte();

void usart_init(){
	
	UBRR0H = BRRV>>8;
	UBRR0L = BRRV;
	UCSR0A = 0x00;
	UCSR0B = 1<<RXEN0 | 1<<TXEN0; //enable rx, tx
	UCSR0C = 1 << UCSZ01 | 1 << UCSZ00; // 1 stop bits, 8 bit data
	
}

void usart_tx_byte(uint8_t data){

	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

uint8_t usart_rx_byte(){
	
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

#endif