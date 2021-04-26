#define F_CPU 16000000UL
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

char buffer0[256];
int position0;
int ladies_total_count;
int i;
int num_ladies;
bool ladiesbafu_closed;
int ladies_bath_flag;
volatile int pulsecount_ladies;
int ladiesbafu_count;
int num_ladiesbafu;
int timer_interrupt=0;
int flowmeter_max=50;
bool waiting_for_gentsbafu_door_to_open;
bool waiting_for_ladiesbafu_door_to_open;


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
void enable_interrupts()
{
	//Magnetic reed switch-ladies bathroom door
	EIMSK|=(1<<INT4);//external interrupt request 0 enable
	EICRB|=(1<<ISC40);//trigger INTO 0n any edge. Interrupt 0 sense control
	
// 	Magnetic reed switch-ladies bathroom door
// 		EIMSK|=(1<<INT4);//external interrupt request 0 enable
// 		EICRB|=(1<<ISC41)|(0<<ISC40);//trigger INTO 0n falling edge. Interrupt 0 sense control
	
	//water flow sensor-ladies shower
	EIMSK|=(1<<INT5);//external interrupt request 0 enable
	EICRB|=(1<<ISC51)|(1<<ISC50);//trigger INTO 0n rising edge. Interrupt 0 sense control

}
// void TIMER_BEGIN()
// {
// 	//16-bit timer
// 	OCR1AL=62500;//62500*256=16MHz
// 	OCR1AH=62500>>8;
// 	TIMSK1=(1<<OCIE1A);
// 	TCCR1B=(1<<WGM12)|(1<<CS12);//prescaler of 256
// }
void start_timer()
{
	OCR1AL=62500;//62500*256=16MHz
	OCR1AH=62500>>8;
	TIMSK1=(1<<OCIE1A);
	TCCR1B=(1<<WGM12)|(1<<CS12);//prescaler of 256	
}
int main(void)
{
	UART0_BEGIN();
	//TIMER_BEGIN();
	enable_interrupts();
	sei();
	while (1)
	{
		for(i=0; i<15; i++)
		{
			_delay_ms(1000);
		}
		num_ladiesbafu=ladiesbafu_count;
		ladiesbafu_count=0;
		
		char ch3[10];
		itoa(num_ladiesbafu,ch3,10);
		
		UART0_TRANSMIT(ch3);
	}
}
ISR(USART0_RX_vect)
{
	
	buffer0[position0]=UDR0;
	position0++;
}

ISR(INT4_vect)
{//ladies bathroom door-reed switch
	if (timer_interrupt == 0)
	{
		timer_interrupt=1;
		//start timer
		start_timer();
	}
}
ISR(INT5_vect)
{//ladies shower-water flow sensor
	if ((ladiesbafu_closed == true) && (waiting_for_ladiesbafu_door_to_open == false))
	{
		pulsecount_ladies++;
		if (pulsecount_ladies > flowmeter_max)
		{
			ladiesbafu_count++;
			
			waiting_for_ladiesbafu_door_to_open=true;
		}
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(((PIND) && (1<<PIND4)) == (0x00))//reed switch interrupt pin_____door open
	{
		ladiesbafu_closed=false;
		waiting_for_ladiesbafu_door_to_open=false;
	}
	else
	{
		ladiesbafu_closed=true;
		
	}
	timer_interrupt=false;
	//disable timer interrupt
	TCCR1B=(1<<WGM12);
}


