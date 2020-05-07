#include "sys.h"
#include "delay.h"
#include "uart.h"	  
#include "can.h"
#include "HAL_can.h"
CanTxMsg CAN_TX_Config ={TX_CANID,CAN_ID_STD,CAN_DATA_FRAME,8,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
CanPeliRxMsg CanPeliRxMsgStructure;
/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� �������󣬽��յ����Ĳ���ӡ
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{
  delay_init();
  uart_initwBaudRate(115200);	 	//���ڳ�ʼ��Ϊ115200
  CANM_Init();
  CAN_Config(CAN_250K,ExtendedFrame_DoubleFilter,0x172,0x325,0x00,0x00);		//CAN�������μĴ�������λ����Ϊ��أ���ֻ�ɽ������մ���Ĵ����е�����CAN_ID
  while(1)
  {
    Send_CANFrame(&CAN_TX_Config);								//����һ֡CAN����
    delay_ms(300);
  }
}




