#include "led_sw.h"

// LED가 연결된 포트 A를 출력으로 설정 & 모든 LED off
void led_init(void)
{
	DDRA = 0xFF;
	PORTA = 0x0;	// 모든 LED off
}

// 선택된 LED on
void led_on(byte bLed)
{
	PORTA = PORTA | bLed;	// LED on
}

// 선택된 LED off
void led_off(byte bLed)
{
	PORTA = PORTA & (~bLed);	// LED off
}

// 선택된 LED on --> off
void led_onoff(byte bLed, float fSec)
{
	double dMsec = fSec / 0.001;	// [sec] --> [msec]

	PORTA = PORTA | bLed;	// LED on
	_delay_ms(dMsec);
	PORTA = PORTA & (~bLed);	// LED off
}

// LED8부터 LED1까지 순서대로 1회 on --> off
void led_test(void)
{
	byte l;
	l = LED8;
	for(int i = 0; i < 8; i++) {
		led_onoff(l, 0.05);
		l <<= 1;
	}
}

// 스위치가 연결된 포트 PE4와 PE5를 입력으로 설정
void sw_init(void)
{
	SFIOR &= ~0x4;  // 모든 포트의 Pull-up 저항 연결 허용
	DDRE &= ~SW1 & ~SW2;	// 1110_1111 & 1101_1111
	PORTE |= SW1 & SW2;	// 포트 PE4와 PE5의 Pull-up 저항 연결
}

// 선택된 스위치가 on(약 50[msec])되면 TRUE를 반환
// 그렇지 않으면 FALSE를 반환
int sw_on(byte bSw)
{
	static int iPrevSwOnOff[2] = {FALSE, FALSE};  // 스위치의 과거 상태
	int iSwOn = FALSE;
	int iSwIndex, iCrntSwOn = !(PINE & bSw);
	
	if(bSw == SW1)			iSwIndex = 0;
	else if(bSw == SW2)	iSwIndex = 1;
	else 						return FALSE;

	// 과거 상태가 off 이고 현재 상태가 on 이면
	if(!iPrevSwOnOff[iSwIndex] && iCrntSwOn) {
		_delay_ms(50);
		// 50[msec] 이후의 스위치의 상태
		iCrntSwOn = !(PINE & bSw);
		iSwOn = iCrntSwOn;
	}
  
	iPrevSwOnOff[iSwIndex] = iCrntSwOn;
	return iSwOn;
}

// 선택된 스위치가 on(약 50[msec] 이상) --> off되면 TRUE를 반환
// 그렇지 않으면 FALSE를 반환
int sw_onoff(byte bSw)
{
	int iOnTimeCnt = 0;

	if(bSw != SW1 && bSw != SW2)	return FALSE;

	// 스위치 off이면
	if(PINE & bSw)	return FALSE;	

	while(TRUE) {
		_delay_ms(10);
		// 스위치 on 상태이면
		if(!(PINE & bSw))			iOnTimeCnt++;
		// 스위치 off 상태이고 50[msec] 이상 스위치가 on 상태에 있었으면
		else if(iOnTimeCnt >= 5) 
			return TRUE;
		else	
			return FALSE;
	}
}

void sw_on_test(void)
{
	// 스위치 1 On/Off --> LED 1~4 깜박
	while(TRUE) {
		if(sw_on(SW1)) {
			led_onoff(LED1_4, 500);
			break;
		}
	}
	// 스위치 2 On/Off --> LED 5~8 깜박
	while(TRUE) {
		if(sw_on(SW2)) {
			led_onoff(LED5_8, 500);
			break;
		}
	}
} 
void sw_onoff_test(void)
{
	// 스위치 1 On/Off --> LED 1~4 깜박
	while(TRUE) {
		if(sw_onoff(SW1)) {
			led_onoff(LED1_4, 500);
			break;
		}
	}
	// 스위치 2 On/Off --> LED 5~8 깜박
	while(TRUE) {
		if(sw_onoff(SW2)) {
			led_onoff(LED5_8, 500);
			break;
		}
	}
} 