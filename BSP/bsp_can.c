/*******************************************
	*�ļ��� ��  bsp_can.c
	*��   �ߣ�  WF
	*�޸�ʱ�䣺 2021.03.26
	*��   ����  v1.0
  *˵   ����  CAN����Դ�ļ�
*******************************************/

#include "bsp_can.h"


/***********************************************************************
													CAN����˽�к���
***********************************************************************/

/* 
	*��������CAN_GPIO_Config()
	*��  �ܣ�CAN�������ú���
	*��  �ߣ�WF
	*��  ������
	*����ֵ����
	*ʱ  �䣺2021.03.26
*/
static void CAN_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ��CAN��TX��RX����ʱ�� */
	RCC_APB2PeriphClockCmd(CAN_RX_CLK|CAN_TX_CLK, ENABLE);
	
	/* CAN_RX��������Ϊ������������������ */
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_RX_PORT, &GPIO_InitStructure);
	
	/* CAN_TX��������Ϊ���츴����� */
	GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_TX_PORT, &GPIO_InitStructure);
}

/* 
	*��������CAN_NVIC_Config()
	*��  �ܣ�CAN�ж����ú���
	*��  �ߣ�WF
	*��  ������
	*����ֵ����
	*ʱ  �䣺2021.03.26
*/
static void CAN_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* ����Ϊ�ж����ȼ�����1 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* �ж����� */
	NVIC_InitStructure.NVIC_IRQChannel = CAN_RX_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_Init(&NVIC_InitStructure);
}

/* 
	*��������CAN_Mode_Config()
	*��  �ܣ�CANģʽ���ú���
	*��  �ߣ�WF
	*��  ������
	*����ֵ����
	*ʱ  �䣺2021.03.26
*/
static void CAN_Mode_Config(void)
{
	CAN_InitTypeDef CAN_InitStructure;
	
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
	
	/* CAN register init */
  CAN_DeInit(CANx);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;           //�ر�ʱ�䴥��ͨ��ģʽ
  CAN_InitStructure.CAN_ABOM=ENABLE;           //�ر��Զ����߹رչ���
  CAN_InitStructure.CAN_AWUM=ENABLE;           //�ر��Զ�����ģʽ
  CAN_InitStructure.CAN_NART=DISABLE;           //�رս�ֹ�Զ��ش�����ʹ���Զ��ش���
  CAN_InitStructure.CAN_RFLM=DISABLE;           //�ڽ������ʱFIFOδ������
  CAN_InitStructure.CAN_TXFP=DISABLE;           //���ȼ��ɱ��ĵı�ʶ��������
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;   //����ģʽ
  
  /* Baudrate = 125kbps*/
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;
  CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
  CAN_InitStructure.CAN_Prescaler=48;
  CAN_Init(CANx, &CAN_InitStructure);
}

/* 
	*��������CAN_Mode_Config()
	*��  �ܣ�CANɸѡ�����ú���
	*��  �ߣ�WF
	*��  ������
	*����ֵ����
	*ʱ  �䣺2021.03.26
*/
static void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	/* CAN filter init */

  CAN_FilterInitStructure.CAN_FilterNumber=0;                   //ɸѡ����0
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //����λģʽ
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//32λɸѡ��
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;              //ɸѡ����16λ
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;               //ɸѡ����16λ
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;          //ɸѡ����16λ������
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;           //ɸѡ����16λ������
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;           //ɸѡ��������FIFO0
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;          //ʹ��ɸѡ��
  CAN_FilterInit(&CAN_FilterInitStructure);
	
	/* CAN FIFO0 message pending interrupt enable */ 
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}


/*********************************************************************
												  CAN����ӿ�
**********************************************************************/

/* 
	*��������CAN_Config()
	*��  �ܣ�CAN���ú���
	*��  �ߣ�WF
	*��  ������
	*����ֵ����
	*ʱ  �䣺2021.03.26
*/
void CAN_Config(void)
{
	CAN_WakeUp(CAN1);
	CAN_GPIO_Config();
	CAN_NVIC_Config();
	CAN_Mode_Config();
	CAN_Filter_Config();
}

/* 
	*��������CAN_SendMsg()
	*��  �ܣ�CAN����֡����
	*��  �ߣ�WF
  *��  ����u32 Ext_ID: ��չ��ʶ�� ,u8* zdata������ָ��, u8 len�����ݳ���
	*����ֵ��PASSED������ɹ���FAILED:ʧ��
	*ʱ  �䣺2022.10.5 
*/
uint8_t CAN_SendMsg(u32 Ext_ID ,u8* zdata, u8 len)
{
	uint32_t i = 0;
	uint8_t TransmitMailbox = 0;

	CanTxMsg TxMessage;
	TxMessage.StdId=0;
	TxMessage.ExtId=Ext_ID;
	TxMessage.IDE=CAN_ID_EXT;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=len;
	
	/* ����������д�뷢������ */
	for(i = 0; i < len; i++)
	{
		TxMessage.Data[i] = zdata[i];
	}
	
	TransmitMailbox = CAN_Transmit(CANx, &TxMessage);
	
	/* �ȴ�������� */
  while((CAN_TransmitStatus(CANx, TransmitMailbox) != CANTXOK) && (i != 0xFFFF))
  {
    i++;
  }
	
	if(i == 0xFFFF) //���䳬ʱ
	{
		return FAILED;
	}
	else //����ɹ�
	{
		return PASSED;
	}
}

/* 
	*��������CAN_ReciveMES()
	*��  �ܣ�CAN����֡����
	*��  �ߣ�WF
	*��  ����u8* zdata������ָ��, u8 len�����ݳ���
	*����ֵ��PASSED������ɹ���FAILED:ʧ��
	*ʱ  �䣺2021.03.26
*/
uint8_t CAN_ReciveMsg(u8 *zdata, u8 len)
{
	uint32_t i = 0;
	CanRxMsg RxMessage;
	
	/* �ȴ�������𣨽��յ���Ϣ�� */
	i = 0;
  while((CAN_MessagePending(CANx, CAN_FIFO0) < 1) && (i != 0xFFFF))
  {
    i++;
  }

  /* receive */
  RxMessage.StdId=0x00;
  RxMessage.IDE=CAN_ID_STD;
  RxMessage.DLC=0;
  RxMessage.Data[0]=0x00;
  RxMessage.Data[1]=0x00;
	
  CAN_Receive(CANx, CAN_FIFO0, &RxMessage);

  if (RxMessage.StdId!=0x1998)
  {
    return FAILED;  
  }

  if (RxMessage.IDE!=CAN_ID_STD)
  {
    return FAILED;
  }

  if (RxMessage.DLC!=len)
  {
    return FAILED;  
  }
	
  return PASSED; /* CAN_Recive Passed */
}
