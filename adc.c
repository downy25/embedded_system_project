#include "adc.h"

// ATmega128 ADC 초기 설정
void adc_init(void)
{
	DDRF = 0x0;	// 포트 F(ADC7~0) 모두 입력으로 설정
	ADMUX = ADC_AREF;	// 기준 전압: AREF 핀
	ADCSRA = ADC_EN | ADC_CLK_DIV_128;	// single conversion, no interrupt, prescaler = 2 
}

// 선택된 채널에 대해 ADC 변환 결과 반환
// ADC7-0 single ended input만 대응
float adc_get_single(byte bCh)
{
	if( (ADMUX & 0x1F) != bCh )
		ADMUX |= (bCh & 0x7);	// single ended input 선택
	ADCSRA |= ADC_START;
	while( !(ADCSRA & ADC_DONE));	// 변환 완료?

	return (ADCW * VOLT_AREF / 1024.0);
}

// 선택된 채널에 대해 ADC 변환 결과 반환
// differential input만 대응
float adc_get_diff(byte bCh)
{
	float fGain;
	unsigned int uiAdc;

	if( (ADMUX & 0x1F) != bCh )
		ADMUX |= (bCh & 0x1F);	// differential input 선택
	ADCSRA |= ADC_START;
	while( !(ADCSRA & ADC_DONE));	// 변환 완료?

	uiAdc = ADCW;
	switch(bCh) {
		case ADC_3_2_X200: fGain = 200.0; break;
		case ADC_3_2_X10:  fGain = 10.0;  break;
		default: fGain = 1.0;
	}

	if(uiAdc & 0x200) { // 음수
		uiAdc = 0x200 - (uiAdc & 0x1FF);
		return -(uiAdc * VOLT_AREF / (fGain * 512.0) );
	} else {	// 양수
		return (uiAdc * VOLT_AREF / (fGain * 512.0) );
	}
} 
