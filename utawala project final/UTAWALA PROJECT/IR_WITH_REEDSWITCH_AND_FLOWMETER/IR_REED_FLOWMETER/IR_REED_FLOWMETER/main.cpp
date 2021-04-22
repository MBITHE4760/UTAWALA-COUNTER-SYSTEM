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
int ladies_total_count;
int gents_total_count;
int i;
int num_ladies;
int num_gents;
bool gentsbafu_closed;
bool ladiesbafu_closed;
int gents_bath_flag;
int ladies_bath_flag;
volatile int pulsecount_ladies;
volatile int pulsecount_gents;
int gentsbafu_count;
int ladiesbafu_count;
int num_gentsbafu;
int num_ladiesbafu;
char url[100];
char gents_total[]="&gentstotal=";
char gents_bafu[]="&gentsbafu=";
char ladies_bafu[]="&ladiesbafu=";
char url1[]="AT+HTTPPARA=\"URL\",\"www.fedha.ke/http/insert.php?ladiestotal=";
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

void clearbuffer1()
{
	for (position1=0;position1<256;position1++)
	{
		buffer1[position1]=0x00;
	}
	position1=0;
}

void enable_interrupts()
{
	//IR-ladies entrance
EIMSK|=(1<<INT0);//external interrupt request 0 enable
EICRA|=(1<<ISC01)|(0<<ISC00);//trigger INTO 0n falling edge. Interrupt 0 sense control

//IR-gents entrance
EIMSK|=(1<<INT1);//external interrupt request 0 enable
EICRA|=(1<<ISC11)|(0<<ISC10);//trigger INTO 0n falling edge. Interrupt 0 sense control

//Magnetic reed switch-gents bathroom door
EIMSK|=(1<<INT2);//external interrupt request 0 enable
EICRA|=(1<<ISC21)|(1<<ISC20);//trigger INTO 0n rising edge. Interrupt 0 sense control

//water flow sensor-gents shower
EIMSK|=(1<<INT3);//external interrupt request 0 enable
EICRA|=(1<<ISC31)|(1<<ISC30);//trigger INTO 0n rising edge. Interrupt 0 sense control

//Magnetic reed switch-ladies bathroom door
EIMSK|=(1<<INT4);//external interrupt request 0 enable
EICRA|=(1<<ISC41)|(1<<ISC40);//trigger INTO 0n rising edge. Interrupt 0 sense control

//water flow sensor-ladies shower
EIMSK|=(1<<INT5);//external interrupt request 0 enable
EICRA|=(1<<ISC51)|(1<<ISC50);//trigger INTO 0n rising edge. Interrupt 0 sense control

}

int main(void)
{
	UART0_BEGIN();
	UART1_BEGIN();
	enable_interrupts();
	sei();
	while (1)
	{
		for(i=0; i<60; i++)
		{//1 minute delay
			_delay_ms(1000);
		}
		num_ladies=ladies_total_count;
		ladies_total_count=0;
		num_gents=gents_total_count;
		gents_total_count=0;
		num_gentsbafu=gentsbafu_count;
		gentsbafu_count=0;
		num_ladiesbafu=ladiesbafu_count;
		ladiesbafu_count=0;
		
		if ((ladies_total_count !=0) || (gents_total_count!=0) || (gentsbafu_count !=0) || (ladiesbafu_count !=0))
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




