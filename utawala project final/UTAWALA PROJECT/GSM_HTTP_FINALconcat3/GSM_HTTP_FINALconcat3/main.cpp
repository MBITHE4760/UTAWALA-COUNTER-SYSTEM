#define F_CPU 16000000UL
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

char buffer0[256];
char buffer1[256];
int position0;
int position1;
int count;
int i;
int num_people;
char url[100];
char name[]="winny";
char url1[]="AT+HTTPPARA=\"URL\",\"www.fedha.ke/http/insert.php?count=";
char url2[]="\"\r\n";

char AT[]="AT\r";
char CONTYPE[]="AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n";
char APN[]="AT+SAPBR=3,1,\"APN\",\"internet\"\r\n";
char USER[]="AT+SAPBR=3,1,\"USER\",\"\"\r\n";
char PWD[]="AT+SAPBR=3,1,\"PWD\",\"\"\r\n";
char open_GPRS[]="AT+SAPBR=1,1\r\n";
char query_GPRS[]="AT+SAPBR=2,1\r\n";
char http_init[]="AT+HTTPINIT\r\n";
char http_para[]="AT+HTTPPARA=\"CID\",1\r\n";
char start_GET[]="AT+HTTPACTION=0\r\n";
char http_read[]="AT+HTTPREAD\r\n";
char http_term[]="AT+HTTPTERM\r\n";
char close_GPRS[]="AT+SAPBR=0,1\r\n";


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
void clearbuffer0()
{
	for (position0=0;position0<256;position0++)
	{
		buffer0[position0]=0x00;
	}
	position0=0;
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
	DDRD = 1 << 0;
	
	EIMSK|=(1<<INT0);//external interrupt request 0 enable
	EICRA|=(1<<ISC01)|(0<<ISC00);//trigger INTO 0n falling edge. Interrupt 0 sense control
	sei();
	while (1)
	{
		for(i=0; i<60; i++)
		{
			_delay_ms(1000);
		}
		num_people=count;
		count=0;
		if (count !=0)
		{
			char ch[10];
			itoa(num_people,ch,10);
			
				_delay_us(1);
				strcpy(url,url1);
				strcat(url,ch);
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
	count++;
}





