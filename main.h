#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL  // 하드웨어 동작 주파수 반영
#define __DELAY_BACKWARD_COMPATIBLE__	// _delay_ms() 함수와 _delay_us() 관련 컴파일러 호환성 문제

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TRUE	1
#define FALSE	0

// 사용자 정의 자료형
typedef unsigned char	byte;	// 8비트
typedef unsigned short	word;	// 16비트
typedef unsigned int	uint;
 
#endif /* MAIN_H_ */