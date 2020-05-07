#include "sys.h"
#include "delay.h"
#include "uart.h"	  
#include "can.h"
#include "HAL_can.h"
#include "tim2.h" 
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
  Tim2_UPCount_test(47,1000); //1msʱ��
  while(1)
  {
    if(Time_Flag_1s)																	//ÿ���ѯ����ӡһ��
    {
      Time_Flag_1s = 0;
      CAN_Peli_Receive(&CanPeliRxMsgStructure);			//�������һ�γɹ���CANͨ�ŵ���Ϣ��CanPeliRxMsgStructure�ṹ����
      printf("CANID:0x%x  Data",CanPeliRxMsgStructure.ID);	//���ڴ�ӡ
      for(i=0;i<8;i++)															
      {
        printf("%x",CanPeliRxMsgStructure.Data[i]);		//��ӡCAN��������
      }
      printf("\r\n");//����
    }
  }
}




