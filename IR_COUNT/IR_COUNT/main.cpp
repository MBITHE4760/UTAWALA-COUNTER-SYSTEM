#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
int i;
int num_people;
int bathroomcount;
volatile int pulsecount;
bool doorclosed;
int bath_flag;
int total_count;
int all_people;
char total_people[]="Total Count:";
char bafu_count[]="Bafu Count:";
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
	EIMSK|=(1<<INT0);//external interrupt request 0 enable
	EICRA|=(1<<ISC01)|(1<<ISC00);//trigger INTO 0n rising edge. Interrupt 0 sense control
	EIMSK|=(1<<INT1);//external interrupt request 1 enable
	EICRA|=(1<<ISC11)|(1<<ISC10);//trigger INTO 0n rising edge. Interrupt 0 sense control
	EIMSK|=(1<<INT2);//external interrupt request 1 enable
	EICRA|=(1<<ISC21)|(0<<ISC20);//trigger INTO 0n falling edge. Interrupt 0 sense control
	doorclosed=false;
	bath_flag=0;
	sei();
	while (1){
		
	for (i=0;i<20;i++)
	{
		_delay_ms(1000);
	}
		num_people=bathroomcount;
		bathroomcount=0;
		all_people=total_count;
		total_count=0;
		
		char ch[10];
		itoa(num_people,ch,10);
		
		char ch1[10];
		itoa(all_people,ch1,10);
		UART0_TRANSMIT(total_people);
		UART0_TRANSMIT(ch1);
		UART0_TRANSMIT("\n");
		
		UART0_TRANSMIT(bafu_count);
		UART0_TRANSMIT(ch);
		UART0_TRANSMIT("\n");
	}
}
//reed switch
ISR(INT0_vect)
{
 doorclosed=true;
 bath_flag=0;
}
//flow meter
ISR(INT1_vect)
{
	pulsecount++;
	if ((pulsecount>100) && (doorclosed == true) && (bath_flag == 0))
	{
		bath_flag=1;
		bathroomcount++;
	}
}

ISR(INT2_vect)
{
	//IR SENSOR.
	total_count++;
}