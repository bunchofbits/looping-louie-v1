/*
 * ll_init.c
 *
 * Created: 31.10.2015 10:34:20
 *  Author: Simon
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Looping Louie.h"
#include "ll_init.h"

void ll_initSystem( struct pinPortMap *ledMap )
{
	uint8_t i;
	
	// player 1 - red
	ledMap[0].pin = PA0;
	ledMap[0].port = &PORTA;
	ledMap[0].direct = DDRA;
	// player 1 - green
	ledMap[1].pin = PA1;
	ledMap[1].port = &PORTA;
	ledMap[1].direct = DDRA;
	// player 1 - blue
	ledMap[2].pin = PA2;
	ledMap[2].port =&PORTA;
	ledMap[2].direct = DDRA;
	
	// player 2 - red
	ledMap[3].pin = PA3;
	ledMap[3].port = &PORTA;
	ledMap[3].direct = DDRA;
	// player 2 - green
	ledMap[4].pin = PA4;
	ledMap[4].port = &PORTA;
	ledMap[4].direct = DDRA;
	// player 2 - blue
	ledMap[5].pin = PA5;
	ledMap[5].port = &PORTA;
	ledMap[5].direct = DDRA;
	
	// player 3 - red
	ledMap[6].pin = PA6;
	ledMap[6].port = &PORTA;
	ledMap[6].direct = DDRA;
	// player 3 - green
	ledMap[7].pin = PA7;
	ledMap[7].port = &PORTA;
	ledMap[7].direct = DDRA;
	// player 3 - blue
	ledMap[8].pin = PC7;
	ledMap[8].port = &PORTC;
	ledMap[8].direct = DDRC;
	
	// player 4 - red
	ledMap[9].pin = PC6;
	ledMap[9].port = &PORTC;
	ledMap[9].direct = DDRC;
	// player 4 - green
	ledMap[10].pin = PC5;
	ledMap[10].port = &PORTC;
	ledMap[10].direct = DDRC;
	// player 4 - blue
	ledMap[11].pin = PC4;
	ledMap[11].port = &PORTC;
	ledMap[11].direct = DDRC;
	
	// player 5 - red
	ledMap[12].pin = PC3;
	ledMap[12].port = &PORTC;
	ledMap[12].direct = DDRC;
	// player 5 - green
	ledMap[13].pin = PC2;
	ledMap[13].port = &PORTC;
	ledMap[13].direct = DDRC;
	// player 5 - blue
	ledMap[14].pin = PC1;
	ledMap[14].port = &PORTC;
	ledMap[14].direct = DDRC;
	
	// player 6 - red
	ledMap[15].pin = PC0;
	ledMap[15].port = &PORTC;
	ledMap[15].direct = DDRC;
	// player 6 - green
	ledMap[16].pin = PD7;
	ledMap[16].port = &PORTD;
	ledMap[16].direct = DDRD;
	// player 6 - blue
	ledMap[17].pin = PG2;
	ledMap[17].port = &PORTG;
	ledMap[17].direct = DDRG;
	
	// player 7 - red
	ledMap[18].pin = PG1;
	ledMap[18].port = &PORTG;
	ledMap[18].direct = DDRG;
	// player 7 - green
	ledMap[19].pin = PG0;
	ledMap[19].port = &PORTG;
	ledMap[19].direct = DDRG;
	// player 7 - blue
	ledMap[20].pin = PL7;
	ledMap[20].port = &PORTL;
	ledMap[20].direct = DDRL;
	
	// player 8 - red
	ledMap[21].pin = PL6;
	ledMap[21].port = &PORTL;
	ledMap[21].direct = DDRL;
	// player 8 - green
	ledMap[22].pin = PL5;
	ledMap[22].port = &PORTL;
	ledMap[22].direct = DDRL;
	// player 8 - blue
	ledMap[23].pin = PL4;
	ledMap[23].port = &PORTL;
	ledMap[23].direct = DDRL;
	
	for( i = 0; i < MAX_PLAYERS * 3; i++)
	{
		ledMap[i].direct |= (1<<ledMap[i].pin);
	}
	
	DDRB |= (1<<DDB5); // Looping Louie Speaker
	
	DDRH |= (1<<DDH5); // Motor output
	
	sei(); // turn on global interrupts
}