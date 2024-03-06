#include "host_cmd.h"
#include "led_sw.h"
#include "usart0.h"
#include "adc.h"
#include "fnd.h"
#include "i2c_temp.h"
#include "buzzer_tc0.h"

char caRxBuf[RX_BUF_LEN];	// ���ڿ� ���� ����
int iRxBufCnt = 0;
char caCmd[CMD_LEN];		// ���۵Ǵ� ��� ����
int iData;					// ��ɰ� �Բ� ���۵Ǵ� ���� ������ ����

// ȣ��Ʈ PC�κ��� ���ŵǴ� ���ڸ� �޾Ƹ��� ����
// ���ŵ� ���ڿ��� caRxBuf[]�� ����
// caRxBuf[]�� ������ ��ɾ�� �����ͷ� �и�
// ��ɾ�� caCmd[]�� ����, �����ʹ� iData�� ����
void get_cmd(void)
{
	while(1) {
		char cRx = usart0_getchar();
		if(iRxBufCnt >= (RX_BUF_LEN - 1)) {	// ���ڿ� ���� ���� �뷮 �ʰ�
			caRxBuf[RX_BUF_LEN-1] = '\0';
			iRxBufCnt = 0;
			usart0_putchar('\n', NULL); usart0_putchar('\r', NULL);
			return;
		} else if(cRx == '\b') {	// 'backspace' ����
			if(iRxBufCnt > 0) {
				usart0_putchar('\b', NULL); usart0_putchar(' ', NULL); usart0_putchar('\b', NULL);
				iRxBufCnt--;	// ���ڿ� ���� ���ۿ��� �ֱ� ���� �����
			}
		} else if(cRx == '\n' || cRx == '\r') {		// ���ڿ� ���� �Ϸ�
			usart0_putchar('\n', NULL); usart0_putchar('\r', NULL);
			caRxBuf[iRxBufCnt] = '\0';
			// ���ŵ� ���ڿ��� ��ɰ� �����ͷ� �����Ͽ� ����
			int r = 0, c = 0;

			while(isblank(caRxBuf[r])) r++;	// ���� ������ �պκ� ���� ����
			while(caRxBuf[r]) {		// ��� ����
				if(isblank(caRxBuf[r]))	break;
				else caCmd[c++] = caRxBuf[r++];
			}
			caCmd[c] = '\0';
			while(isblank(caRxBuf[r])) r++;	// ��ɰ� ������ ������ ���� ����

			if (r < iRxBufCnt) {	// ������ ���ڿ� ó��
				if(caRxBuf[r] == '0' && caRxBuf[r+1] == 'x') {	// "0x" --> 16����
					iData = strtol(&caRxBuf[r+2], NULL, 16);
				} else // 10����
					iData = atoi(&caRxBuf[r]);
			} else iData = 0;
			iRxBufCnt = 0;
	
			if(strlen(caCmd) > 0) printf("\t CMD: %s, DATA(10): %d\n\r", caCmd, iData);
			return;
		} else {
			caRxBuf[iRxBufCnt++] = cRx;
			usart0_putchar(cRx, NULL);
		}
	}
}

// ȣ��Ʈ PC�� �͹̳� ���α׷� ȭ�� ����� �ʱ� �޼��� ���
void clear(void)
{
	printf("\033[H\033[J"); // �͹̳� ȭ�� �����
	printf("=== ATmega128 command program(%s %s) ===\n\r", __TIME__, __DATE__);
}

// ���� ��ɾ� ���� ���
void help(void)
{
	printf("==== Example Commands ====\n\r");
	printf("-- Command \t -- Description \n\r");
	printf("- help \t\t display these help messages \n\r");
	printf("- clear \t clear terminal screen \n\r");
	printf("- led_test \t led test \n\r");
	printf("- sw_test \t switch test(SW1 on/off --> SW2 on/off) \n\r");
	printf("- led 0x?? \t led_onoff(0x??, 0.5) \n\r");
	printf("- mycmd \t print I am Ok! \n\r");
	printf("- myled1 \t myled1 ? \n\r");
	printf("- myled2 \t myled2 0x1234 0x12, 0x34 \n\r");
}

// caCmd[]�� ����� ��ɾ�� iData�� ����� �����͸� �̿��Ͽ� ��ɾ �����ϴ� �Լ� ȣ��
void do_cmd(void)
{
	if(strlen(caCmd) == 0) ;
	else if(strcmp(caCmd, "help") == 0) help();
	else if(strcmp(caCmd, "clear") == 0) clear();
	else if(strcmp(caCmd, "led_test") == 0) led_test();
	else if(strcmp(caCmd, "sw_test") == 0) sw_on_test();
	else if(strcmp(caCmd, "led") == 0) led_onoff(iData, 0.5);
	else if(strcmp(caCmd,"mycmd")==0) mycmd();
	else if(strcmp(caCmd,"myled1")==0) myled1(iData);
	else if(strcmp(caCmd,"myled2")==0) myled2(iData);
	else if(strcmp(caCmd,"adc_show")==0) adc_show();
	else if(strcmp(caCmd,"I2Cshow")==0) I2Cshow();
	else if(strcmp(caCmd,"adcAvg")==0)adcAvg();
	else	printf("Unknown command!!! \n\r");
}

//ȣ��Ʈ PC�� I am Ok�� �۽��ϴ� �Լ�
void mycmd(void){
	printf("I am OK!");
}
// LED5-8�� �ڿ� ���ڸ�ŭ ����Ű�� ���
void myled1(int count){
	for(int i=0;i<count;i++){
		led_onoff(LED5_8,0.5);
		_delay_ms(500);
	}
}
void myled2(int data){
	int data_array[]={data>>8,data&0xFF};
	
	for(int i=0;i<4;i++){
		led_onoff(data_array[i%2],0.5);
		_delay_ms(500);
	}
}
void adc_show(void){
	float fAdcVal,crntVal,prvVal;
	int count=0,crnt_num,prv_num;
	byte LED[]={0x0,LED1,LED2,LED3,LED4,LED5,LED6,LED7,LED8};
	byte iLed=0x0;
	led_init();
	adc_init();
	fnd_init();
	
	while(TRUE) {
		fAdcVal = adc_get_single(ADC7);			// single ended input
		fnd_set(fAdcVal);
		if(count==0){
			prv_num = (int)(fAdcVal/0.5);
			prvVal=fAdcVal;
			for(int i=0;i<prv_num;i++) iLed |= LED[i];
		}
		else{
			crnt_num=(int)(fAdcVal/0.5);
			crntVal=fAdcVal;
			int diff_num=crnt_num-prv_num;
			float diffVal=crntVal-prvVal;
			
			if(diffVal>0) for(int i=0;i<=prv_num+diff_num;i++) iLed|=LED[i];
			else if(diffVal<0){
				iLed=0x0;
				for(int i=0;i<=prv_num+diff_num;i++) iLed|=LED[i];
			}
			else{
				for(int i=0;i<prv_num;i++) iLed |= LED[i];
			}
			prv_num=crnt_num;
			prvVal=crntVal;
		}
		count++;
		for(int i=0;i<10;i++){
			led_on(iLed);
			fnd_show();
			led_off(iLed);
			fnd_show();
		}
	}
}

void I2Cshow(void){
	byte bConfig;
	float fTempVal,f1sTemp;
	int ledonoff=FALSE;
	int delay=20;
	
	led_init();
	fnd_init();
	i2c_init();

	// ������ ��ȯ ���е�: 11��Ʈ, default�� 9��Ʈ
	if(i2c_aTS75_set_temp_res(I2C_aTS75_RES_12)) led_onoff(LED5_8, 0.3);
	else { led_on(LED_ALL); return; }
	
	// Configuration �������� ����
	if(i2c_aTS75_sel_reg(I2C_aTS75_REG_CONFIG)) led_onoff(LED1_4, 0.3);
	else { led_on(LED_ALL); return; }
	
	// Configuration �������� �б�
	if(i2c_aTS75_rd_reg(&bConfig)) led_onoff(LED5_8, 0.3);
	else { led_on(LED_ALL); return; }
	
	if(!i2c_aTS75_sel_reg(I2C_aTS75_REG_TEMP)){led_on(LED_ALL); return;} //aTS75 ���� Ư���� �����Ϸ� ���� ������ �ʿ��� ��찡 ����
	_delay_ms(1);

	// Configuration �������� ���� LED�� ���
	led_onoff(bConfig, 1);

	// Temperature �������� ����
	if(i2c_aTS75_sel_reg(I2C_aTS75_REG_TEMP)) led_onoff(LED1_4, 0.3);
	else { led_on(LED_ALL); return; }
		
	f1sTemp=i2c_aTS75_get_temp(); //���ۿµ��� ����
	while(TRUE) {
		// Temperature �������� �� �о �Ǽ��� ��ȯ
		fTempVal = i2c_aTS75_get_temp();
		fnd_set(fTempVal);
		if((fTempVal-f1sTemp)>=1&&(fTempVal-f1sTemp)<2){
			ledonoff=TRUE; 
			delay=40;
		}
		else if((fTempVal-f1sTemp)>=2){
			ledonoff=TRUE; 
			delay=20;
		}
		else{
			ledonoff=FALSE;
		}
		
		if(ledonoff) led_on(LED_ALL);
		for(int i=0; i<delay; i++) fnd_show();	// fnd_show() 100ȸ ȣ�� --> �� 500[msec]
		if(ledonoff)led_off(LED_ALL);
		for(int i=0; i<delay; i++) fnd_show();	// fnd_show() 100ȸ ȣ�� --> �� 500[msec]
	}
}
void adcAvg(void){
	led_init();
	adc_init();
	fnd_init();
	float fAdcVal,fAdcValSum=0.0,fAdcValAvg;
	int count=0,avgCount=0;
	
	while(TRUE){
		fAdcVal = adc_get_single(ADC7);			// single ended input
		fAdcValSum+=fAdcVal;
		
		if(count++==10){
			fAdcValAvg=fAdcValSum/count;
			avgCount++;
			printf("Avg %03d voltage %f [v] \n\r",avgCount,fAdcValAvg);
			count=0;
			fAdcValSum=0.0;
		}
		_delay_ms(50);
	}
}