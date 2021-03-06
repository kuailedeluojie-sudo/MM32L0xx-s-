#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//开发板
//LED驱动代码	   
////////////////////////////////////////////////////////////////////////////////// 	   
void LED_Init(void)
{
    
    GPIO_InitTypeDef  GPIO_InitStructure;
 	
    
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);  //开启GPIOA,GPIOB时钟
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	LED1_OFF();
	LED2_OFF();
	LED3_OFF();
	LED4_OFF();
}
 
