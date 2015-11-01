/*
 * ll_debug.c
 *
 * Created: 31.10.2015 10:56:59
 *  Author: Simon
 */ 

#include <avr/io.h>

#include "Looping Louie.h"
#include "ll_debug.h"

void __inline__ setDebugLED(uint8_t enable)
{
#ifdef DEBUG
	if(enable)
	{
		PORTB |= (1<<PB7);
	}
	else 
	{
		PORTB &= ~(1<<PB7);
	}
#endif
}

void __inline__ setDebugSpeaker(uint8_t enable)
{
#ifdef DEBUG
	ll_setSpeaker(enable);
#endif
}
void __inline__ initDebugSystem()
{
#ifdef DEBUG
	DDRB |= (1<<DDB7) | (1<<DDB5); // set Debug[LED|Speaker] as output
#endif
}