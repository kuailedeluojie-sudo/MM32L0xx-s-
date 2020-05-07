#include "sys.h"
#include "delay.h"
#include "uart.h"	  
#include "can.h"
#include "HAL_can.h"
CanPeliRxMsg CanPeliRxMsgStructure;
/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� �������󣬽��յ����Ĳ���ӡ
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{
  u8 i = 0;
  delay_init();
  uart_initwBaudRate(115200);	 	//���ڳ�ʼ��Ϊ115200
  CANM_Init();
  CAN_Config(CAN_250K,ExtendedFrame_DoubleFilter,0x172,0x325,0x00,0x00);		//CAN�������μĴ�������λ����Ϊ��أ���ֻ�ɽ������մ���Ĵ����е�����CAN_ID
  while(1)
  {
    if(flag)																	
    {
      flag = 0;
      printf("CANID:0x%x  Data",CanPeliRxMsgStructure.ID);	//���ڴ�ӡ
      for(i=0;i<8;i++)															
      {
        printf("%x",CanPeliRxMsgStructure.Data[i]);		//��ӡCAN��������
      }
      printf("\r\n");																		//����
    }
  }
}




