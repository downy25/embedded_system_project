#ifndef LED_SW_H_
#define LED_SW_H_

#include "main.h"

#define LED1		(1 << 7)
#define LED2		(1 << 6)
#define LED3		(1 << 5)
#define LED4		(1 << 4)
#define LED5		(1 << 3)
#define LED6		(1 << 2)
#define LED7		(1 << 1)
#define LED8		(1 << 0)
#define LED1_4		(0xF0)
#define LED5_8		(0x0F)
#define LED_ALL	(0xFF)

#define SW1			(1 << 4)
#define SW2			(1 << 5)

// LED 관련 함수 원형
void led_init(void);
void led_on(byte bLed);
void led_off(byte bLed);
void led_onoff(byte bLed, float fSec);
void led_test(void);

// 스위치 관련 함수 원형
void sw_init(void);
int sw_on(byte bSw);
int sw_onoff(byte bSw);
void sw_on_test(void);
void sw_onoff_test(void);
 
#endif /* LED_SW_H_ */