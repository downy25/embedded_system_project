#include "fnd.h"

// 숫자 --> 7-세그먼트
byte baNumToFnd[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,	// 0, 1, 2, 3, 4
					0x6D, 0x7D, 0x27, 0x7F, 0x6F,	// 5, 6, 7, 8, 9
					0x80, 0x40};					// ., -
// 각 7-세그먼트에 출력될 값					
byte baFnd[4];

// FND가 연결된 포트 C와 G를 출력으로 설정
void fnd_init(void)
{
	DDRC = 0xFF;
	PORTC = 0x0;
	DDRG = 0xF;
	PORTG = 0x0;
}

// baFnd[] 값 결정 --> 각 7-세그먼트에 표시될 내용을 설정
// 표시 범위: 9999. ~ -999.
void fnd_set(float fVal)
{
	int i, iDigitNum, iPos, iDisp = FALSE;
	float f10s;

	if (fVal > 9999) {	// "9999." 표시
		for (i = 0; i < 4; i++) baFnd[i] = baNumToFnd[9];// '9'
		baFnd[3] |= baNumToFnd[10];	// 소수점
	} else if (fVal < -999) {	// "-999." 표시
		baFnd[0] = baNumToFnd[11];	// - 부호
		for (i = 1; i<4; i++) baFnd[i] = baNumToFnd[9];// '9'
		baFnd[3] |= baNumToFnd[10];	// 소수점
	} else {
		if (fVal >= 0) {// 0 ~ 9999.
			iPos = 0;	// 숫자 표시 시작 위치
			f10s = 1000.;	// 천 단위부터 처리
		} else {		// -999 ~ 0
			baFnd[0] = baNumToFnd[11];	// - 부호
			iPos = 1;// 숫자 표시 시작 위치
			f10s = 100.;// 백 단위부터 처리
			fVal = -fVal;	// 절대값
		}
		// '-'부호 이외의 숫자 표시
		while (iPos <= 4) { 
			iDigitNum = fVal / f10s;
			if(iDigitNum > 0 || (int)f10s == 1)	iDisp = TRUE;
			if(iDisp) {	// 앞자리 '0'은 표시 제외, ex) "0012." --> "12.00"
				baFnd[iPos] = baNumToFnd[iDigitNum];
				if ((int)f10s == 1) baFnd[iPos] |= baNumToFnd[10];	// 소수점
				iPos++;
			}
			fVal -= iDigitNum * f10s;
			f10s /= 10.;
		}
	}
}

// 각 7-세그먼트 순차로 5[msec] 동안 on
void fnd_show(void)
{
	static byte bPortG = 0x8;	// 정적 변수
	static int iPos = 0;			// 정적 변수

	PORTC = baFnd[iPos];
	if(++iPos == 4) iPos = 0;

	PORTG = bPortG;
	bPortG >>= 1;
	if(bPortG == 0) bPortG = 0x8;

	_delay_ms(5);
}
 