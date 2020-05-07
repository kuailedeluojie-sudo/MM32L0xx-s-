#include "key.h"
#include "usb.h"	  
#include "HAL_device.h"
#include "stdio.h"
#include "led.h"

//for sprintf start
char printBuf[100];
void Uart_ConfigInit(u32 bound);
void UartSendGroup(u8* buf,u16 len);
void UartSendAscii(char *str);
//UartSendGroup((u8*)printfBuf, sprintf(printfBuf,"sprintf ok\r\n"));//��ӡ��ʽ
//end 

/*main.c�ļ��ں�������*/
void CRS_USB_Config(void);
void DelayMs(u32 ulMs);                             
void NVIC_Configuration(void);
void SysInit(void);
void GPIO_Configuration(void);
void SetUSBSysClockTo48M(void);
#define printf


int main(void)
{
	u32 i;
  SetUSBSysClockTo48M();//USB��Ҫ48Mʱ��
	Uart_ConfigInit(9600);	                            //���ڳ�ʼ��Ϊ9600
	GPIO_Configuration();
	KEY_Init();
	LED_Init();	 
    
	usb_test();
	while(1)                                        
	{
		if(GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_0))//����PA0����
		{
			for(i=0;i<5000;i++);
			if(GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_0))
			{
			LED1_TOGGLE();
			keyBoard_ctrl(0x00,0x00,4,0x00,0x00,0x00,0x00);
			while(GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_0));
			}
		}
	}	
 }

 /////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
eg.
CRS_USB_Config();
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////	
void CRS_USB_Config(void)
{
    /* Select USB SOF as synchronization source */
    //  CRS_SynchronizationSourceConfig(CRS_SYNCSource_USB);
    CRS->CFGR &= 0xcfffffff; //
    /*Enables the automatic hardware adjustment of TRIM bits: AUTOTRIMEN:*/
    CRS->CR |= 0x2<<28; //GPIO

    CRS->CR |= 0x40; //autoen
    /*Enables the oscillator clock for frequency error counter CEN*/
    CRS->CR |= 0x20; //cen
}
 
/********************************************************************************************************
**������Ϣ ��void DelayMs(u32 ulMs)                         
**�������� ���Ǿ�ȷ��ʱ
**������� ��u32 ulMs ����
**������� ����
**��    ע ��ϵͳʱ��Ϊ72MHzʱ,����Ϊ1000ʱԼ��ʱ1s��,��ϵͳ���Ŀ��ʵ�����j��ֵ
             ����һЩ��ʱҪ�󲻸ߵĳ���
********************************************************************************************************/
void DelayMs(u32 ulMs)
{
	u32 i;
	u16 j;
	for(i = ulMs;i > 0;i--)
	{
		for(j = 4700;j > 0;j--);
	}
}
/**
  * @brief  
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); 
    
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*��PA1����Ϊģ������*/
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/**
  * @brief  Configures Vector Table base location.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  /*�����ж����ȼ�ģʽ,0λ��ռ���ȼ�,4λ�����ȼ�*/
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
}
/********************************************************************************************************
**������Ϣ ��SysInit(void)                        
**�������� �����ϵͳ��ʼ��
**������� ����
**������� ����
**    ��ע ��ϵͳʱ�ӳ�ʼ�� 96MHz=AHP=PCLK2=2*PCLK1
             ��ɴ��ڳ�ʼ�� ������9600,����żУ��,��Ӳ��������,1λֹͣλ
********************************************************************************************************/

void SetUSBSysClockTo48M(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
	RCC_DeInit();
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /* Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;
    /* Flash 0 wait state ,bit0~2*/
    FLASH->ACR &= ~0x07;
		FLASH->ACR |=0x02;
    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
      
    /*  PLL configuration:  = (HSE ) * (5+1) = 48MHz */
		RCC->CFGR &= (uint32_t)0xFFFCFFFF;
		RCC->CR &= (uint32_t)0x000FFFFF;  
		 
		RCC->CFGR |= (uint32_t ) RCC_CFGR_PLLSRC ;
		RCC->CR |= 0x14000000;//pll = 6/1
    //RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    //RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLXTPRE_HSE_Div2 | RCC_CFGR_PLLMULL6);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
eg.
GPIO_ConfigInit();
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO_ConfigInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*��PA1����Ϊģ������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
eg.
Uart_ConfigInit(9600);
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Uart_ConfigInit(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);

    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	

    UART_Init(UART1, &UART_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);        

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    UART_Cmd(UART1, ENABLE);                  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
eg.
UartSendByte(0x66);
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void UartSendByte(u8 dat)
{
    UART_SendData( UART1, dat);
    while(UART_GetFlagStatus(UART1,UART_FLAG_TXEPT));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
eg.
u8 buf[66];
UartSendGroup(buf,6);
eg2.
u8 printfBuf[66];
UartSendGroup((u8*)printfBuf, sprintf(printfBuf,"sprintf ok\r\n"));
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void UartSendGroup(u8* buf,u16 len)
{
    while(len--)
        UartSendByte(*buf++);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
eg1.
char buf[66];
UartSendAscii(buf);
eg2.
char buf[66];
UartSendAscii("hello world\r\n");
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void UartSendAscii(char *str)
{
    while(*str)
        UartSendByte(*str++);
}

