#include "wkup.h"
#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������
//�������� ����	   
//////////////////////////////////////////////////////////////////////////////////

void Sys_Stop(void)
{  
    PWR_EnterSTOPMode(0, PWR_STOPEntry_WFI);
    
}
//////////////////////////////////////////////////////////////////////////////////	 
//��HSI��Ϊϵͳʱ��  
//////////////////////////////////////////////////////////////////////////////////
void HSI_SYSCLK(void)
{
    RCC_HSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)==RESET);
    RCC->CFGR &=~ 0xf;
    while((RCC->CFGR&0xf) != 0x0);
}	

/********************************************************************************************************
**������Ϣ ��EXTI0_IRQHandler(void)                         
**�������� ���ⲿ�ж�0�����������жϱ�־λ
**������� ����
**������� ����
********************************************************************************************************/
void EXTI0_1_IRQHandler(void)
{ 		    		    				     		    
    EXTI_ClearITPendingBit(EXTI_Line0); // ���LINE10�ϵ��жϱ�־λ		  
} 

/********************************************************************************************************
**������Ϣ ��WKUP_Init(void)                         
**�������� ���ⲿ�жϻ��Ѵ�����ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void WKUP_Init(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;  		  
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��GPIOA�͸��ù���ʱ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA.0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��IO
    //ʹ���ⲿ�жϷ�ʽ
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,  EXTI_PinSource0);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//���ð������е��ⲿ��·
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���EXTI_Trigger_Falling;//
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);//��ʼ���ⲿ�ж�
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;//��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
    
    PWR_WakeUpPinCmd(ENABLE); 
}

