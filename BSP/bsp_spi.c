/*******************************************
	*�ļ��� ��  bsp_spi.c
	*��   �ߣ�  ����
	*�޸�ʱ�䣺 2021.02.20
	*��   ����  v1.0
    *˵   ����  Ӳ��SPI����Դ�ļ�
*******************************************/


#include "bsp_spi.h"



/*
	*�������� MySPI_Init()
	*��  �ܣ� SPI��ʼ������
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.02.20
*/
void MySPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;
	
	//ʹ��SPI��GPIOʱ��
	USER_SPI_APBX_CLOCK_FUN(USER_SPI_APBX_CLOCK , ENABLE);
	USER_SPI_GPIO_APBxClock_FUN(USER_SPI_GPIO_APBxClock , ENABLE);
	
	/****�������Ź���ģʽ****/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//ʱ������
	GPIO_InitStructure.GPIO_Pin = USER_SPI_CLK_PIN  ;
	GPIO_Init(USER_SPI_CLK_PORT , &GPIO_InitStructure);
	//MISO����
	GPIO_InitStructure.GPIO_Pin = USER_SPI_MISO_PIN  ;
	GPIO_Init(USER_SPI_MISO_PORT , &GPIO_InitStructure);
	//MOSI����
	GPIO_InitStructure.GPIO_Pin = USER_SPI_MOSI_PIN  ;
	GPIO_Init(USER_SPI_MOSI_PORT , &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);              //PB13/14/15����
	
	//����SPI����ģʽ
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;           //���� SPI ������Ԥ��ƵֵΪ2�������ٶ�Ϊ 36Mhz/2
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                                 //�ڴ���ͬ��ʱ�ӵĵڶ������ز���
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                                  //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CRCPolynomial = 6;                                     //����CRCУ�����ʽ�����ͨ�ſɿ��ԣ�����1����
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                            //8λ����֡ģʽ
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;           //˫��˫��ȫ˫��
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                           //�������ݴ���Ϊ���λ��ǰ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                                //SPI����ģʽ
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                                    //ͨ���������NSS����
	SPI_Init(USER_SPI_X , &SPI_InitStructure);
	
	//ʹ��SPIx
	SPI_Cmd(USER_SPI_X , ENABLE);
	
}


/*
	*�������� SPI_ReadWriteByte()
	*��  �ܣ� SPI���ͽ����ֽں���
	*��  ���� zdata���ֽ�����
	*����ֵ�� ��������
	*��  �ߣ� ����
	*��  �ڣ� 2021.02.20
*/
u8 SPI_ReadWriteByte(u8 zdata)
{
	u8 times = 0;
	//�ȴ���������
	while(SPI_I2S_GetFlagStatus(USER_SPI_X , SPI_I2S_FLAG_TXE) == RESET){
		times++;
		if(times > 200) break;
	}        
	SPI_I2S_SendData(USER_SPI_X , zdata);
	
	times = 0;
	//�ȴ�������һ�� byte
	while(SPI_I2S_GetFlagStatus(USER_SPI_X , SPI_I2S_FLAG_RXNE) == RESET){
		times++;
		if(times > 200) break;
	}     
	return SPI_I2S_ReceiveData(USER_SPI_X);                                  //����ͨ��SPI2������յ�����
}


/*
	*�������� SPI_SetSpeed()
	*��  �ܣ� SPI���ô����ٶȺ���
	*��  ���� SPI_BaudRatePrescaler��������Ԥ��Ƶֵ
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.02.20
*/
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1 &= 0XFFC7;
	SPI2->CR1 |= SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
	SPI_Cmd(USER_SPI_X , ENABLE); 
}


