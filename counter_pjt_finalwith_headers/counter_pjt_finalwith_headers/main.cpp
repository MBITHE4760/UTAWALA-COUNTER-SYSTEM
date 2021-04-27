#define F_CPU 16000000UL
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "gsm.h"
#include "interrupts.h"

void UART0_BEGIN()
{
	UBRR0H=0;
	UBRR0L=103;
	UCSR0B|=(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);
}
void UART3_BEGIN()
{
	UBRR3H=0;
	UBRR3L=103;
	UCSR3B|=(1<<RXEN3)|(1<<TXEN3)|(1<<RXCIE3);
	UCSR3C|=(1<<UCSZ30)|(1<<UCSZ31);
}


int main(void)
{
	UART0_BEGIN();
	UART3_BEGIN();
	enable_interrupts();
	sei();
	while (1)
	{
		for(i=0; i<60; i++)
		{//1 minute delay
			_delay_ms(1000);
		}
			send_count();

	}
}

ISR(INT0_vect)
{//ladies entrance
	ladies_total_count++;
}
ISR(INT1_vect)
{//gents entrance
	gents_total_count++;
}
ISR(INT2_vect)
{//gents bathroom door
	gentsbafu_closed=true;
	gents_bath_flag=0;
}
ISR(INT3_vect)
{//gents shower-water flow sensor
	pulsecount_gents++;
	if ((pulsecount_gents>100) && (gentsbafu_closed == true) && (gents_bath_flag == 0))
	{
		gents_bath_flag=1;
		gentsbafu_count++;
	}
}
ISR(INT4_vect)
{//ladies bathroom door
	ladiesbafu_closed=true;
	ladies_bath_flag=0;
}
ISR(INT5_vect)
{//ladies shower-water flow sensor
	pulsecount_ladies++;
	if ((pulsecount_ladies>100) && (ladiesbafu_closed == true) && (ladies_bath_flag == 0))
	{
		ladies_bath_flag=1;
		ladiesbafu_count++;
	}
}





