/*
 * Looping_Louie.h
 *
 * Created: 31.10.2015 10:39:54
 *  Author: Simon
 */ 


#ifndef LOOPING_LOUIE_H_
#define LOOPING_LOUIE_H_

#ifndef F_CPU
#define F_CPU				16000000UL // 16 MHz clock speed
#endif

#define MAX_PLAYERS			8

#define SET_BIT(PORT,PIN)	(PORT |= (1<<PIN))
#define CLR_BIT(PORT,PIN)	(PORT &= ~(1<<PIN))

#define MOTOR_PORT			PORTH
#define MOTOR_PIN			PH5

#define HIGH				1
#define LOW					0

#include <avr/io.h>

struct pinPortMap
{
	volatile uint8_t *port;
	uint8_t pin;
	uint8_t direct;
};

void ll_setSpeaker(uint8_t enable);

#endif /* LOOPING_LOUIE_H_ */