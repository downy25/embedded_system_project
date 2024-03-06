#ifndef HOST_CMD_H_
#define HOST_CMD_H_

#include "main.h"

#define RX_BUF_LEN	(30 + 1)	// "명령어 + 데이터" 문자열 최대 길이: 30
#define CMD_LEN		(15 + 1)	// 명령어 최대 길이: 15

void get_cmd(void);
void do_cmd(void);
void clear(void);
void help(void);
/*void mycmd(void);
void myled1(int count);
void myled2(int data);
void adc_show(void);
void I2Cshow(void);
void adcAvg(void);*/

#endif /* HOST_CMD_H_ */
