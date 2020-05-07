#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "adc.h"
#include "led.h"

/********************************************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
int main(void)
{
    u16 ADCVAL;
    float Temp;
    delay_init();
    LED_Init();
    uart_initwBaudRate(115200);	 //���ڳ�ʼ��Ϊ115200
    
    /*����Ϊ�ڲ��¶ȴ�����*/
    ADC1_SingleChannel( ADC_Channel_10);
    while(1)
    {
        ADCVAL=Get_Adc_Average(ADC_Channel_10,5);
        Temp=27.0+(ADCVAL-1800)/5.96;
        printf("�ڲ��¶�=%.2f��\r\n",Temp);
        delay_ms(1000);  
    }
}
