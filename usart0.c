#include "usart0.h"

// USART0 설정 : 9600bps, 8 Data, 1 Stop, No Parity
void usart0_init(void)
{
  	UBRR0H = 103 >> 8;// Baud rate = 9600 bps @ 16[MHz]
  	UBRR0L = 103;		// Baud rate = 9600 bps @ 16[MHz]
	UCSR0A = 0x00;		// 0000_0000: 전송 속도=1배, 멀티프로세서 통신=X
	UCSR0B = 0x18;		// 0001_1000: Rx & Tx 허용
	UCSR0C = 0x06;		// 0000_0110: Async. , No Parity, 1 Stop bit, 8 Data bits
}

// 수신 버퍼(UDR0)에서 한 개 문자를 읽어서 반환
// 주의: 수신 버퍼가 비워질 때까지 무한 루프 수행
char usart0_getchar(void)
{
	while( (UCSR0A & RX_COMPLETE) == 0 );

	return(UDR0);
}

// 송신 버퍼(UDR0)에 한 개 문자를 기입
// FILE *stream: 사용되지는 않지만 printf() 함수와 연결을 위해 꼭 필요
// 주의: 송신 버퍼가 비워질 때까지 무한 루프 수행
void usart0_putchar(char cData, FILE *stream)
{
	if(cData == '\n') {
		while( (UCSR0A & DATA_REG_EMPTY) == 0 );
		UDR0 = '\r';
	}

	while( (UCSR0A & DATA_REG_EMPTY) == 0 );
	UDR0 = cData;
}
 