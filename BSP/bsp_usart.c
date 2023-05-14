/*******************************************
	*�ļ��� ��  bsp_usart.c
	*��   �ߣ�  ����
	*�޸�ʱ�䣺 2021.04.09
	*��   ����  v1.0
  *˵   ����  ��������Դ�ļ�
*******************************************/



#include "bsp_usart.h"



#if DEBUG_USART1

/*
	*�������� Usart1_Init()
	*��  �ܣ� ����1��ʼ������
	*��  ���� BaudRate��������
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.02.19
*/
void Usart1_Init(u32 BaudRate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
    
	DEBUG_USART1_GPIO_APBxClkCmd(DEBUG_USART1_GPIO_CLK, ENABLE);  //ʹ��GPIOʱ��
	DEBUG_USART1_APBxClkCmd(DEBUG_USART1_CLK, ENABLE);            //ʹ��USART1ʱ��
	
  //����USART1��TXΪ�����������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;               
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART1_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART1_TX_GPIO_PORT, &GPIO_InitStructure);
	
  //����USART1��RXΪ��������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;    
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART1_RX_GPIO_PIN;
	GPIO_Init(DEBUG_USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;                  //����USART1Ϊ���ͽ���ģʽ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//û��Ӳ����
	USART_InitStructure.USART_Parity=USART_Parity_No;                            //û����żУ��
	USART_InitStructure.USART_StopBits=USART_StopBits_1;                         //1��ֹͣλ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;                    //8������λ
	USART_InitStructure.USART_BaudRate=BaudRate;                                 //���ò�����
  USART_Init(USART1, &USART_InitStructure);                                    //��ʼ��USART1
	
	NVIC_InitStructure.NVIC_IRQChannel=DEBUG_USART1_IRQ;                         //�����ж�ͨ��ΪUSART1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;                      //������ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;                             //������Ӧ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                                //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                                              //��ʼ���ж�
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);                                 //����USART1Ϊ�����ж�
	USART_Cmd(USART1, ENABLE);                                                   //ʹ��USART1
}

#endif

#if DEBUG_USART2

/*
	*�������� Usart2_Init()
	*��  �ܣ� ����2��ʼ������
	*��  ���� BaudRate��������
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.04.09
*/
void Usart2_Init(u32 BaudRate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
    
	DEBUG_USART2_GPIO_APBxClkCmd(DEBUG_USART2_GPIO_CLK, ENABLE);  //ʹ��GPIOʱ��
	DEBUG_USART2_APBxClkCmd(DEBUG_USART2_CLK, ENABLE);            //ʹ��USART2ʱ��
	
  //����USART2��TXΪ�����������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;               
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART2_TX_GPIO_PORT, &GPIO_InitStructure);
	
  //����USART2��RXΪ��������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;    
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART2_RX_GPIO_PIN;
	GPIO_Init(DEBUG_USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;                  //����USART2Ϊ���ͽ���ģʽ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//û��Ӳ����
	USART_InitStructure.USART_Parity=USART_Parity_No;                            //û����żУ��
	USART_InitStructure.USART_StopBits=USART_StopBits_1;                         //1��ֹͣλ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;                    //8������λ
	USART_InitStructure.USART_BaudRate=BaudRate;                                 //���ò�����
  USART_Init(USART2, &USART_InitStructure);                                    //��ʼ��USART2
	
	NVIC_InitStructure.NVIC_IRQChannel=DEBUG_USART2_IRQ;                          //�����ж�ͨ��ΪUSART2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;                      //������ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;                             //������Ӧ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                                //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                                              //��ʼ���ж�
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);                                 //����USART2Ϊ�����ж�
	USART_Cmd(USART2, ENABLE);                                                   //ʹ��USART2
}

#endif



#if DEBUG_USART3

/*
	*�������� Usart3_Init()
	*��  �ܣ� ����3��ʼ������
	*��  ���� BaudRate��������
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.04.09
*/
void Usart3_Init(u32 BaudRate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
    
	DEBUG_USART3_GPIO_APBxClkCmd(DEBUG_USART3_GPIO_CLK, ENABLE);  //ʹ��GPIOʱ��
	DEBUG_USART3_APBxClkCmd(DEBUG_USART3_CLK, ENABLE);            //ʹ��USART3ʱ��
	
  //����USART3��TXΪ�����������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;               
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART3_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART3_TX_GPIO_PORT, &GPIO_InitStructure);
	
  //����USART3��RXΪ��������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;    
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART3_RX_GPIO_PIN;
	GPIO_Init(DEBUG_USART3_RX_GPIO_PORT, &GPIO_InitStructure);
	
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;                  //����USART3Ϊ���ͽ���ģʽ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//û��Ӳ����
	USART_InitStructure.USART_Parity=USART_Parity_No;                            //û����żУ��
	USART_InitStructure.USART_StopBits=USART_StopBits_1;                         //1��ֹͣλ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;                    //8������λ
	USART_InitStructure.USART_BaudRate=BaudRate;                                 //���ò�����
  USART_Init(USART3, &USART_InitStructure);                                    //��ʼ��USART3
	
	NVIC_InitStructure.NVIC_IRQChannel=DEBUG_USART3_IRQ;                          //�����ж�ͨ��ΪUSART3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;                      //������ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;                             //������Ӧ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                                //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                                              //��ʼ���ж�
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);                                 //����USART3Ϊ�����ж�
	USART_Cmd(USART3, ENABLE);                                                   //ʹ��USART3
}

#endif






/*
	*�������� Usart_SendByte()
	*��  �ܣ� ���ڷ����ֽں���
	*��  ���� pUsart_x�����ڽṹ�壬 zdata���ֽ�����
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.02.19
*/
void Usart_SendByte(USART_TypeDef* pUsart_x, u8 zdata)
{
    USART_SendData(pUsart_x, zdata);
    while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TXE) == RESET);                //�ȴ��������ݼĴ�����
}


/*
	*�������� Usart_SendHalfWord()
	*��  �ܣ� ���ڷ���16λ���ݺ���
	*��  ���� pUsart_x�����ڽṹ�壬 zdata��16λ����
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.02.19
*/
void Usart_SendHalfWord(USART_TypeDef* pUsart_x, u16 zdata)
{
    USART_SendData(pUsart_x, (u8)zdata&0x00ff);                                   //���͸�8λ����                              
    while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TXE) == RESET);                
    USART_SendData(pUsart_x, (u8)zdata&0xff00);                                   //���͵�8λ����
    while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TXE) == RESET);                
}


/*
	*�������� Usart_SendArray()
	*��  �ܣ� ���ڷ����������ݺ���
	*��  ���� pUsart_x�����ڽṹ�壬 array���������飬 Num�����ݸ���
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.02.19
*/
void Usart_SendArray(USART_TypeDef* pUsart_x, u8* array, u8 Num)
{  
    while(Num--)
    {
        USART_SendData(pUsart_x, *array);
        while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TXE) == RESET); 
        array++;
    }
    while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TC) == RESET);                  //�ȴ��������
}


/*
	*�������� Usart_SendString()
	*��  �ܣ� ���ڷ����ַ������ݺ���
	*��  ���� pUsart_x�����ڽṹ�壬 str���ַ���
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.02.19
*/
void Usart_SendString(USART_TypeDef* pUsart_x, u8* str)
{
    while(*str != '\0')
    {
        USART_SendData(pUsart_x, *str);
        while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TXE) == RESET);
        str++;
    }
    while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TC) == RESET);                  //�ȴ��������
}

//�ض���printf()������1�����ͣ�
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
    return (ch);
}



//�ض���scanf()������1�����գ�
int fgetc(FILE *f)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET); //�ȵ����ڽ������
    return (int)USART_ReceiveData(USART1);                        //���ؽ��յ�����
}


#if DEBUG_USART1

/*
	*�������� DEBUG_USART1_IRQHandler()
	*��  �ܣ� ����1�жϷ�����
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.02.19
*/
void DEBUG_USART1_IRQHandler(void)
{
    u8 rData = 0;
    if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET) //�����ݼĴ����ǿ�(�����յ�����)
    {
        rData = USART_ReceiveData(USART1);                  //����ȡ�����ݱ��浽rData
        Usart_SendByte(USART1, rData);
    }
}

#endif


#if DEBUG_USART2

/*
	*�������� DEBUG_USART2_IRQHandler()
	*��  �ܣ� ����2�жϷ�����
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.04.09
*/
void DEBUG_USART2_IRQHandler(void)
{
    u8 rData = 0;
    if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET) //�����ݼĴ����ǿ�(�����յ�����)
    {
        rData = USART_ReceiveData(USART2);                  //����ȡ�����ݱ��浽rData
        Usart_SendByte(USART2, rData);
    }
}

#endif


#if DEBUG_USART3

/*
	*�������� DEBUG_USART3_IRQHandler()
	*��  �ܣ� ����3�жϷ�����
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.04.09
*/
void DEBUG_USART3_IRQHandler(void)
{
    u8 rData = 0;
    if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET) //�����ݼĴ����ǿ�(�����յ�����)
    {
        rData = USART_ReceiveData(USART3);                  //����ȡ�����ݱ��浽rData
        Usart_SendByte(USART3, rData);
    }
}

#endif


