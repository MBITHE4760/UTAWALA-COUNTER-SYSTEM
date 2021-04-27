/*
 * timer_test.cpp
 *
 * Created: 4/27/2021 3:23:54 PM
 * Author : mbithe
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>


int num_ladiesbafu;
int count;
void UART0_TRANSMIT(char* data){
	while(*data){
		while(!(UCSR0A & (1<<UDRE0))){}
		UDR0=*data;
		data++;
	}
}
void UART0_BEGIN()
{
	UBRR0H=0;
	UBRR0L=103;
	UCSR0B|=(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);
}

int main(void)
{
	UART0_BEGIN();
	OCR1AL=62500;//62500*256=16MHz //output compare register low
	OCR1AH=62500>>8; // output compare register high
	//OCR1A = 62500;
	TCCR1B |= (1 << WGM12);
	// Set to CTC Mode
	TIMSK1 |= (1 << OCIE1A);
	//Set interrupt on compare match
	TCCR1B |= (1 << CS12);
	// set prescaler to 64 and starts PWM
	sei();
	// enable interrupts
	while (1);
	{
		_delay_us(1);
		// we have a working timer
		num_ladiesbafu=count;
		count=0;
		
		char ch3[10];
		itoa(num_ladiesbafu,ch3,10);
		
		UART0_TRANSMIT(ch3);
		UART0_TRANSMIT("\r\n");
	}
}

ISR (TIMER1_COMPA_vect)
{
	// action to be done every 250us
	count++;
}

