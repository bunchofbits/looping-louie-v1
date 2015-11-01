/*
 * ll_resetSwitch.c
 *
 * Created: 01.11.2015 10:50:07
 *  Author: Simon
 */ 

#include "Looping Louie.h"
#include "ll_resetSwitch.h"

void __inline__ ll_restartSwEnable()
{
	EIMSK |= (1<<INT4);
}

void __inline__ ll_restartSwDisable()
{
	EIMSK &= ~(1<<INT4);
	RESTART_SW_LED = 0;
}

void ll_initRestartSwitch()
{
	// init restart Switch
	
	PORTE |= (1<<PE4); // enable Pullup for
	
	
	// *********** PWM TIMER *************
	DDRH |= (1<< PH6);
	TCCR2A = (1<<WGM21) | (1<<WGM20) ; // select FastPWM Mode
	TCCR2A |= (1<<COM2A1) | (1<<COM2B1); // clear OC2A and OC2B on compare match

	//TCCR2B = (1<<WGM22); // reset timer on OCR2A instead of 255
	TCCR2B |= (1<<CS20); // prescaler to 1

	//OCR2A = 63; // reset counter at value X
	RESTART_SW_LED = 0;
}