#ifndef ADC_H_
#define ADC_H_

#include "main.h"

// ADMUX 레지스터
#define ADC_AREF	0x0

// ADCSRA 레지스터
#define ADC_EN		0x80
#define ADC_START	0x40
#define ADC_DONE	0x10
#define ADC_CLK_DIV_2	0
#define ADC_CLK_DIV_4	2
#define ADC_CLK_DIV_8	3
#define ADC_CLK_DIV_16	4
#define ADC_CLK_DIV_32	5
#define ADC_CLK_DIV_64	6
#define ADC_CLK_DIV_128	7

#define VOLT_AREF	(5.0)

// single ended input
#define ADC0		0
#define ADC1		1
#define ADC2		2
#define ADC3		3
#define ADC4		4
#define ADC5		5
#define ADC6		6
#define ADC7		7

// differential input
#define ADC_3_2_X10		13	// 01101
#define ADC_3_2_X200	15	// 01111
#define ADC_2_1_X1		18	// 10010
#define ADC_3_1_X1		19	// 10011
#define ADC_4_1_X1		20	// 10100
#define ADC_5_1_X1		21	// 10101
#define ADC_6_1_X1		22	// 10110
#define ADC_7_1_X1		23	// 10111
#define ADC_1_2_X1		25	// 11001
#define ADC_3_2_X1		27	// 11011
#define ADC_4_2_X1		28	// 11100
#define ADC_5_2_X1		29	// 11101

void adc_init(void);
float adc_get_single(byte bCh);
float adc_get_diff(byte bCh);

#endif /* ADC_H_ */ 