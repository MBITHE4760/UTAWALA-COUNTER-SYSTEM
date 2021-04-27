/*
 * interrupts.h
 *
 * Created: 4/27/2021 9:31:24 AM
 *  Author: mbithe
 */ 


#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_


void enable_interrupts()
{
	//IR-ladies entrance
	EIMSK|=(1<<INT0);//external interrupt request 0 enable
	EICRA|=(1<<ISC01)|(0<<ISC00);//trigger INTO 0n falling edge. Interrupt 0 sense control

	// 	IR-gents entrance
	EIMSK|=(1<<INT1);//external interrupt request 0 enable
	EICRA|=(1<<ISC11)|(0<<ISC10);//trigger INTO 0n falling edge. Interrupt 0 sense control
	//
	// 	//Magnetic reed switch-gents bathroom door
	EIMSK|=(1<<INT2);//external interrupt request 0 enable
	EICRA|=(0<<ISC21)|(1<<ISC20);//trigger INTO 0n any edge. Interrupt 0 sense control
	//
	// 	//water flow sensor-gents shower
	EIMSK|=(1<<INT3);//external interrupt request 0 enable
	EICRA|=(1<<ISC31)|(1<<ISC30);//trigger INTO 0n rising edge. Interrupt 0 sense control
	//
	// 	//Magnetic reed switch-ladies bathroom door
	EIMSK|=(1<<INT4);//external interrupt request 0 enable
	EICRB|=(0<<ISC41)|(1<<ISC40);//trigger INTO 0n any edge. Interrupt 0 sense control
	//
	// 	//water flow sensor-ladies shower
	EIMSK|=(1<<INT5);//external interrupt request 0 enable
	EICRB|=(1<<ISC51)|(1<<ISC50);//trigger INTO 0n rising edge. Interrupt 0 sense control

}



#endif /* INTERRUPTS_H_ */