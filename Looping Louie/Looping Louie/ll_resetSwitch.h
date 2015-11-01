/*
 * ll_resetSwitch.h
 *
 * Created: 01.11.2015 10:50:27
 *  Author: Simon
 */ 


#ifndef LL_RESETSWITCH_H_
#define LL_RESETSWITCH_H_

#define RESTART_SW_FADE_IN		1
#define RESTART_SW_FADE_OUT		0

#define RESTART_SW_LED_MIN_VAL	30
#define RESTART_SW_LED_MAX_VAL	255

#define RESTART_SW_LED			OCR2B

void ll_initRestartSwitch();
void ll_restartSwSetLedPwm(uint8_t val);

void ll_restartSwEnable();
void ll_restartSwDisable();

#endif /* LL_RESETSWITCH_H_ */