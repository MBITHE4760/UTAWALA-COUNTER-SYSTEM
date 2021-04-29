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
int flowmeter_max;
bool waiting_for_gentsbafu_door_to_open;
bool waiting_for_ladiesbafu_door_to_open;
int int4_;

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
	
	//water flow sensor-ladies shower
	EIMSK|=(1<<INT5);//external interrupt request 0 enable
	EICRB|=(1<<ISC51)|(1<<ISC50);//trigger INTO 0n rising edge. Interrupt 0 sense control

}

void start_timer(){
	TCNT1=0;
	// 	OCR1AL=62500;//62500*256=16MHz //output compare register low
	// 	OCR1AH=62500>>8; // output compare register high
	OCR1A=15625;
	//OCR1AL=15625;//62500*256=16MHz //output compare register low
 	//OCR1AH=15625>>8; // output compare register high
	TCCR1B|=(1<<WGM12); //CTC-clear timer on compare match
	TIMSK1|=(1<<OCIE1A);//timer 1 interrupt mask enable=timer compare interrupt
	TCCR1B|=(1<<CS12)|(1<<CS10); //timer counter control register //prescaler of 1024; the higher the prescaler, the higher the precision;

}

void stop_timer(){
	TIMSK1 &= ~(1<<OCIE1A);
}
int main(void)
{
	int4_=0;
	num_ladiesbafu=0;
	flowmeter_max=50;
	if (!((PINE & (1<<4)) == 0))
	{
		ladiesbafu_closed=false;
		waiting_for_ladiesbafu_door_to_open=false;
	}
	else{
		ladiesbafu_closed=true;
	}
	UART0_BEGIN();
	enable_interrupts();
	sei();
	while (1)
	{
		for(i=0; i<5; i++)
		{
			_delay_ms(1000);
		}
		if (ladiesbafu_closed)
		{
			UART0_TRANSMIT("closed");
			
		}
		else{
			UART0_TRANSMIT("open");
		}
		
		num_ladiesbafu+=ladiesbafu_count;
		ladiesbafu_count=0;

		char ch3[10];
		itoa(num_ladiesbafu,ch3,10);
		UART0_TRANSMIT(buffer0);
		UART0_TRANSMIT(ch3);
		UART0_TRANSMIT("\r\n");
	}
}
ISR(USART0_RX_vect)
{
	
	buffer0[position0]=UDR0;
	position0++;
}

ISR(INT4_vect)
{//ladies bathroom door-reed switch
	//UART0_TRANSMIT("Y");
	if (timer_interrupt==false)
	{
		timer_interrupt=true;
		start_timer();
	}
		
}
ISR(INT5_vect)
{//ladies shower-water flow sensor
	//UART0_TRANSMIT("Z");
	if ((ladiesbafu_closed == true) && (waiting_for_ladiesbafu_door_to_open==false))
	{
		pulsecount_ladies++;
	
		if (pulsecount_ladies>flowmeter_max) 
		{
			ladiesbafu_count++;
			waiting_for_ladiesbafu_door_to_open=true;
			
		}
	}
	//timer_interrupt=false;
}

ISR(TIMER1_COMPA_vect)
{
	//UART0_TRANSMIT("U");
	if (!((PINE & (1<<4)) == 0))
	{
		UART0_TRANSMIT("O");
		ladiesbafu_closed=false;
		waiting_for_ladiesbafu_door_to_open=false;
	}
	else{
		UART0_TRANSMIT("C");
		ladiesbafu_closed=true;
// 		int x;
// 		x=PINE;
// 		char ch[4];
// 		itoa(x,ch,10);
// 		UART0_TRANSMIT(ch);
	}
	timer_interrupt=false;
	//disable timer interrupt
	TIMSK1&=~(1<<OCIE1A);
}


