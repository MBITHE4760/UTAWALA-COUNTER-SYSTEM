#define F_CPU 16000000UL
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "gsm.h"

char buffer0[256];
char buffer1[256];
int position0;
int position1;
int pulsecount_ladies;
int pulsecount_gents;
bool saved_ladies_count;
bool saved_gents_count;
int gentsbafu_count;
int ladiesbafu_count;

volatile bool gentsbafu_closed;
volatile bool ladiesbafu_closed;
int ladiescount;
int gentscount;

int i;
int num_ladies;
int num_gents;
char gents_total[]="&gentstotal=";
char gents_bafu[]="&gentsbafu=";
char ladies_bafu[]="&ladiesbafu=";
int num_gentsbafu;
int num_ladiesbafu;



void UART0_TRANSMIT(char* data){
	while(*data !='\0'){
		while(!(UCSR0A & (1<<UDRE0))){}
		UDR0=*data;
		data++;
	}
}
void UART1_TRANSMIT(char* data){
	while(*data !='\0'){
		while(!(UCSR1A & (1<<UDRE1))){}
		UDR1=*data;
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
void UART1_BEGIN()
{
	UBRR1H=0;
	UBRR1L=103;
	UCSR1B|=(1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1);
	UCSR1C|=(1<<UCSZ10)|(1<<UCSZ11);
}

void clearbuffer1()
{
	for (position1=0;position1<256;position1++)
	{
		buffer1[position1]=0x00;
	}
	position1=0;
}


int main(void)
{
	UART0_BEGIN();
	UART1_BEGIN();
	EIMSK|=(1<<INT0);//external interrupt request 0 enable
	EICRA|=(1<<ISC01)|(0<<ISC00);//trigger INTO 0n falling edge. Interrupt 0 sense control
	sei();
	while (1)
	{
		for(i=0; i<60; i++)
		{
			_delay_ms(1000);
		}
		num_ladies=ladiescount;
		ladiescount=0;
		num_gents=gentscount;
		gentscount=0;
		num_gentsbafu=gentsbafu_count;
		gentsbafu_count=0;
		num_ladiesbafu=ladiesbafu_count;
		ladiesbafu_count=0;
		
		if ((ladiescount !=0) || (gentscount!=0) || (gentsbafu_count !=0) || (ladiesbafu_count !=0))
		{
			char ch[10];
			itoa(num_ladies,ch,10);
			
			char ch1[10];
			itoa(num_gents,ch1,10);
			
			char ch2[10];
			itoa(num_gentsbafu,ch2,10);
			
			char ch3[10];
			itoa(num_ladiesbafu,ch3,10);
			
			_delay_us(1);
			strcpy(url,url1);
			strcat(url,ch);
			strcat(url,gents_total);
			strcat(url,ch1);
			strcat(url,gents_bafu);
			strcat(url,ch2);
			strcat(url,ladies_bafu);
			strcat(url,ch3);
			strcat(url,url2);
			UART0_TRANSMIT(AT);
			UART1_TRANSMIT(AT);
			_delay_ms(2000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(CONTYPE);
			UART1_TRANSMIT(CONTYPE);
			_delay_ms(2000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(APN);
			UART1_TRANSMIT(APN);
			_delay_ms(1000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(USER);
			UART1_TRANSMIT(USER);
			_delay_ms(1000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(PWD);
			UART1_TRANSMIT(PWD);
			_delay_ms(1000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(open_GPRS);
			UART1_TRANSMIT(open_GPRS);
			_delay_ms(5000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(query_GPRS);
			UART1_TRANSMIT(query_GPRS);
			_delay_ms(1000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(http_init);
			UART1_TRANSMIT(http_init);
			_delay_ms(1000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(http_para);
			UART1_TRANSMIT(http_para);
			_delay_ms(1000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(url);
			UART1_TRANSMIT(url);
			_delay_ms(1000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(start_GET);
			UART1_TRANSMIT(start_GET);
			_delay_ms(1000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(http_read);
			UART1_TRANSMIT(http_read);
			_delay_ms(5000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(http_term);
			UART1_TRANSMIT(http_term);
			_delay_ms(1000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
			UART0_TRANSMIT(close_GPRS);
			UART1_TRANSMIT(close_GPRS);
			_delay_ms(1000);
			UART0_TRANSMIT(buffer1);
			clearbuffer1();
		}

	}
}
ISR(USART0_RX_vect)
{
	
	buffer0[position0]=UDR0;
	position0++;
}

ISR(USART1_RX_vect)
{
	
	buffer1[position1]=UDR1;
	position1++;
}
ISR(INT0_vect)
{
	ladiescount++;
}
ISR(INT1_vect)
{
	gentscount++;
}


ISR(INT2_vect)
{
	if(gentsbafu_closed == false)
	{
		gentsbafu_closed=true;
	}
	else
	{
		gentsbafu_closed=false;
	}
}
ISR(INT4_vect)
{
	if(ladiesbafu_closed == false)
	{
		ladiesbafu_closed=true;
	}
	else
	{
		ladiesbafu_closed=false;
	}
}
ISR(INT3_vect)
{
	pulsecount_gents++;
	if((pulsecount_gents >= 20) && (gentsbafu_closed == true) && (saved_gents_count == false))
	{
		gentsbafu_count++;
	}
}

ISR(INT5_vect)
{
	pulsecount_ladies++;
	if((pulsecount_ladies >= 20) && (ladiesbafu_closed == true) && (saved_ladies_count == false))
	{
		ladiesbafu_count++;
	}
}







