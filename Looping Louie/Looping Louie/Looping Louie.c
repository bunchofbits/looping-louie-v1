/*
* Looping Louie.c
*
* Created: 11.10.2015 12:35:54
* Author : Simon Richter
*/

#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Looping Louie.h"
#include "ll_init.h"
#include "ll_debug.h"
#include "ll_resetSwitch.h"

#define LED_COLOR_OFF			0x00000000 // all LEDs OFF
#define LED_COLOR_PLAYER_LOST	0x00ff0000 // RED
#define LED_COLOR_PLAYER_1CHIP	0x000000ff // BLUE
#define LED_COLOR_PLAYER_2CHIP	0x0000ff00 // GREEN
#define LED_COLOR_PLAYER_3CHIP	0x00ffffff // WHITE

enum GAME_STEPS {
	STEP_GAME_ERROR	= 0,
	STEP_GAME_INIT,
	STEP_GAME_RUN,
	STEP_GAME_STOP,
	STEP_GAME_RESTART_WAIT,
#ifdef DEBUG
	LL_DEBUGGING,
#endif
};

uint8_t playerChips[MAX_PLAYERS];
struct pinPortMap g_ledMap[MAX_PLAYERS * 3];
uint8_t g_step;
volatile uint8_t g_isr_portchistory = 0; // all ports are pulled up
uint8_t g_isr_pinState[MAX_PLAYERS];

void ll_setSpeaker(uint8_t enable)
{
	if(enable)
	{
		PORTB |= (1<<PB5);
	}
	else
	{
		PORTB &= ~(1<<PB5);
	}
}
void ll_stopMotor()
{
	MOTOR_PORT &= ~(1<<MOTOR_PIN);
}

void ll_startMotor()
{
	MOTOR_PORT |= (1<<MOTOR_PIN);
}

void ll_setLedColor(uint8_t player, uint32_t color)
{
	uint8_t r,g,b;
	b = color & 0x000000ff;
	color >>= 8;
	g = color & 0x000000ff;
	color >>= 8;
	r = color & 0x000000ff;
	
	if(r > 0)
	{
		//SET_PORT(*g_ledMap[(player * 3)].port, g_ledMap[(player * 3)].pin);
		*g_ledMap[(player * 3)].port |= (1<<g_ledMap[(player * 3)].pin);
	}
	else
	{
		//CLR_PORT(*g_ledMap[(player * 3)].port, g_ledMap[(player * 3)].pin);
		*g_ledMap[(player * 3)].port &= ~(1<<g_ledMap[(player * 3)].pin);
	}
	
	if(g > 0)
	{
		*g_ledMap[(player * 3) +1].port |= (1<<g_ledMap[(player * 3) +1].pin);
	}
	else
	{
		*g_ledMap[(player * 3) +1].port &= ~(1<<g_ledMap[(player * 3) +1].pin);
	}
	
	if(b > 0)
	{
		*g_ledMap[(player * 3) +2].port |= (1<<g_ledMap[(player * 3) +2].pin);
	}
	else
	{
		*g_ledMap[(player * 3) +2].port &= ~(1<<g_ledMap[(player * 3) +2].pin);
	}
}

void ll_updateColor(uint8_t player)
{
	uint32_t color = 0x00ffffff;
	
	switch(playerChips[player])
	{
		case 0:
			color = LED_COLOR_PLAYER_LOST;
			break;
		
		case 1:
			color = LED_COLOR_PLAYER_1CHIP;
			break;
		
		case 2:
			color = LED_COLOR_PLAYER_2CHIP;
			break;
		
		case 3:
			color = LED_COLOR_PLAYER_3CHIP;
			break;
		
		//default:
		// do not catch error ..
	}
	ll_setLedColor(player, color);
}

void ll_stopGame()
{
	uint8_t i;
	// disable interrupt while not playing
	PCICR &= ~(1<<PCIE2);
	
	// stop the motor
	ll_stopMotor();
	
	// switch off all LEDs of every player expect that one, that lost
	for(i = 0; i < MAX_PLAYERS; i++) 
	{
		if(playerChips[i] > 0)
		{
			ll_setLedColor(i, LED_COLOR_OFF);
		}
	}
	
	// activate Restart Switch
	ll_restartSwEnable();
	
	// beep 1 time to signal GAME STOPED
	ll_setSpeaker(1);
	_delay_ms(50);
	ll_setSpeaker(0);
}

void ll_runGame()
{
	return;
}

void ll_startGame()
{
	int i;
	for(i = 0; i < MAX_PLAYERS; i++)
	{
		playerChips[i] = 3; // 3 Chips per player at start
		g_isr_pinState[i] = LOW; // all pins are at LOW
		ll_setLedColor(i, LED_COLOR_PLAYER_3CHIP); // set LED of every player to START
	}
	g_isr_portchistory = 0;
	PCICR |= (1<<PCIE2); // enable interrupt for PCIE2
	PCMSK2 = 0xff; // Interrupt at all 8 pins
	
	ll_startMotor();
}

int main(void)
{
	uint8_t fadeDirect = RESTART_SW_FADE_IN;
	uint8_t fadeBrightness = 0;
	
	initDebugSystem();
	ll_initSystem( g_ledMap );
	ll_initRestartSwitch();
	
	g_step = STEP_GAME_RESTART_WAIT;
	// activate Restart Switch
	ll_restartSwEnable();
	while (1)
	{
		switch(g_step)
		{
			case STEP_GAME_INIT:
			ll_startGame();
			// stop RestartSwitch LED
			RESTART_SW_LED = 0;
			g_step = STEP_GAME_RUN;
			break;
			
			case STEP_GAME_RUN:
			ll_runGame();
			break;
			
			case STEP_GAME_STOP:
			ll_stopGame();
			// start fading in the RestartSwitch LED
			fadeDirect = RESTART_SW_FADE_IN;
			fadeBrightness = 0;
			g_step = STEP_GAME_RESTART_WAIT;
			break;
			
			case STEP_GAME_RESTART_WAIT:
				if(fadeDirect == RESTART_SW_FADE_IN)
				{
					fadeBrightness++;
					if(fadeBrightness >= RESTART_SW_LED_MAX_VAL)
					{
						fadeDirect = RESTART_SW_FADE_OUT;
					}
				} 
				else
				{
					fadeBrightness--;
					if(fadeBrightness <= RESTART_SW_LED_MIN_VAL)
					{
						fadeDirect = RESTART_SW_FADE_IN;
					}
				}
				_delay_ms(10);
				RESTART_SW_LED = fadeBrightness;
				
			break;
						
			default:
			g_step = STEP_GAME_ERROR;
			break;
		}
		// TODO: needed !?!
		_delay_us(1);
	}
}

ISR(PCINT2_vect)
{
	uint8_t changedbits;
	int8_t player = -1;
	uint8_t i;
	
	changedbits = PINK ^ g_isr_portchistory;
	g_isr_portchistory = PINK;
	
	// if nothing changed -> ignore
	if(changedbits == 0)
	{
#ifdef DEBUG
		setDebugLED(1);
		setDebugSpeaker(1);
		_delay_ms(10);
		setDebugLED(0);
		setDebugSpeaker(0);
#endif
		return;
	}
	
	// check which player lost a chip
	for( i = 0; i < MAX_PLAYERS; i++)
	{
		if((changedbits & 0x01) == 1)
		{
			player = i;
			break;
		}
		changedbits >>= 1;
	}
	
	if(g_isr_pinState[player] == LOW)
	{
		g_isr_pinState[player] = HIGH;
	}
	else
	{
		g_isr_pinState[player] = LOW;
		return;
	}
	// disable Interrupt for this player
	//PCMSK2 &= ~(1<<player);
	
	playerChips[player] --;
	if( playerChips[player] <= 0)
	{
		// player lost
		g_step = STEP_GAME_STOP;
	}
	ll_updateColor(player);
	_delay_ms(200);
}

ISR(INT4_vect)
{
	// Restart Switch pressed -> Start game
	g_step = STEP_GAME_INIT;
	
	ll_restartSwDisable();
}