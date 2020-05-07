/**
  ******************************************************************************
  * @file    uart_test.c  
  * @brief   ���ô��ڹ���
  *          
  ******************************************************************************
  */
#include "uart_test.h"
/********************************************************************************************************
**������Ϣ ��UartInit(USART_TypeDef* USARTx)                      
**�������� ����ʼ������
**������� ��USART_TypeDef* USARTx ��ѡ��USART1��USART2��USART3
**������� ����
********************************************************************************************************/

void UartInit(USART_TypeDef* USARTx)
{
	USART_InitTypeDef       USART_InitStructure;  
	GPIO_InitTypeDef  GPIO_InitStructure;   
	
if(USARTx==USART1)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;                                                          //uart1_tx  pa9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                     // ���⸴�����
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;                                                         //uart1_rx  pa10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                                       //��������   
	GPIO_Init(GPIOA, &GPIO_InitStructure);
			
}	
 
if(USARTx==USART2)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;                                                         //uart1_tx  pa2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                    // ���⸴�����
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;                                                         //uart1_rx  pa3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                                      //��������   
	GPIO_Init(GPIOA, &GPIO_InitStructure);
			
}	

if(USARTx==USART3)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;                                                        //uart3_tx  pc10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                    // ���⸴�����
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;                                                        //uart3_rx  pc11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                                      //��������   
	GPIO_Init(GPIOC, &GPIO_InitStructure);
			
}	
		
	USART_InitStructure.USART_BaudRate =  115200;                                                          //115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	
	USART_Init(USARTx, &USART_InitStructure);

	/*UART ģ��ʹ��*/
	USART_Cmd(USARTx, ENABLE);  

	/*����жϱ�־*/
	USART_ClearITPendingBit(USART1, 0xff); 
	USART_ClearITPendingBit(USART2, 0xff); 
}

/********************************************************************************************************
**������Ϣ ��uart_send(USART_TypeDef* USARTx,char c)                    
**�������� �����ڷ����ֽ�
**������� ��USART_TypeDef* USARTx ��ѡ��USART1��USART2��USART3
**������� ��char c ,���ڷ��͵��ֽ�
**������� ����
********************************************************************************************************/

void uart_send(USART_TypeDef* USARTx,char c)
{
	USART_SendData(USARTx,(uint16_t)c);  
	
	while(1)
	{
		if(USART_GetITStatus(USARTx, USART_IT_TXIEN))
	
		{
			 USART_ClearITPendingBit(USARTx, USART_IT_TXIEN);
			 break;
		}
		
	}
}

/********************************************************************************************************
**������Ϣ ��unsigned char inbyte(USART_TypeDef* USARTx)                
**�������� �����ڽ��պ���
**������� ��USART_TypeDef* USARTx ��ѡ��USART1��USART2��USART3
**������� ��unsigned char ���ڽ��շ��ص��ֽ�
********************************************************************************************************/

unsigned char inbyte(USART_TypeDef* USARTx)
{
	unsigned char temp;

	while(1)
	{
		if(USART_GetITStatus(USARTx, USART_IT_RXIEN))
		{
			 /*��������ж�λ*/
			USART_ClearITPendingBit(USARTx, USART_IT_RXIEN); 
			break;
		}	
	}
	temp = (uint8_t)USART_ReceiveData(USARTx); 
	return temp;
}


/********************************************************************************************************
**������Ϣ ��void Uart1RxTest(USART_TypeDef* USARTx)               
**�������� �����ڽ��պ�������
**������� ��USART_TypeDef* USARTx ��ѡ��USART1��USART2��USART3
**������� ����
********************************************************************************************************/

unsigned int Uart1RxTest(USART_TypeDef* USARTx)
{
	unsigned char temp;
	temp = inbyte(USARTx);
	return temp;	
}


/********************************************************************************************************
**������Ϣ ��void outbyte(char c)               
**�������� �����ڷ��ͺ������ԣ�ͨ�����Գ����ն˻��ߴ��������ӡ����
**������� ��char c ���ڷ��͵��ֽ�
**������� ����
********************************************************************************************************/

void outbyte(char c)
{
	/* �������USART2��USART3,�����ú����ĵ�һ���������� */
	uart_send(USART1,c); 
}



