#include "main.h"
#include "led_sw.h"
#include "usart0.h"
#include "host_cmd.h"
#include "i2c_temp.h"

// printf() 함수의 문자열 출력을 USART0로 변경
FILE x_stdout = FDEV_SETUP_STREAM(usart0_putchar, NULL, _FDEV_SETUP_WRITE);

void main(void)
{
	stdout = &x_stdout;	// printf() 함수 출력 관련 설정

	led_init();
	sw_init();
	
	led_onoff(LED_ALL, 0.3);

	// USART0 설정: 9600 bps, no parity, 1 stop bit, 8-bit data
	usart0_init();

	printf("\033[H\033[J"); // 터미널 화면 지우기
	// 프로그램 실행 메세지 출력(컴파일 날짜와 시각 포함)
	printf("=== ATmega128 command program(%s %s) ===\n\r", __TIME__, __DATE__);
	printf("ATmega128>");

	while(1) {
		get_cmd();
		do_cmd();
		printf("\n\rATmega128>");

		//led_onoff(LED_ALL, 0.05);
	}
}