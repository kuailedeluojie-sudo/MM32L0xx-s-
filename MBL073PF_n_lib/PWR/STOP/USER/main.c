#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "wkup.h"
#include "led.h"


int main(void)
{ 
    u8 i;
    u32 j;
    delay_init();
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    
    for(i=0;i<5;i++)
    {
        LED1_TOGGLE();
        delay_ms(300);
    }
    LED1_OFF();
    WKUP_Init();			//��ʼ��WK_UP������ͬʱ����Ƿ���������
    HSI_SYSCLK();//��HSI��Ϊϵͳʱ��
    Sys_Stop();//����STOPģʽ
    
    while(1)
    {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        j = 30000;while(j--);
    }
}

