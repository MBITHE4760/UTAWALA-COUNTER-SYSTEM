#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
int i;
int num_people;
int count;

void UART0_TRANSMIT(char* data){
	while(*data !='\0'){
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
	EIMSK|=(1<<INT2);//external interrupt request 0 enable
	EICRA|=(1<<ISC21)|(0<<ISC20);//trigger INTO 0n rising edge. Interrupt 0 sense control
	
	sei();
	while (1){
		
		for (i=0;i<10;i++)
		{
			_delay_ms(1000);
		}
		num_people=count;
		count=0;
		
		char ch[10];
		itoa(num_people,ch,10);
		UART0_TRANSMIT(ch);
		UART0_TRANSMIT("\n");
		
		
	}
}
//reed switch
ISR(INT2_vect)
{
	count++;
}
//flow meter
