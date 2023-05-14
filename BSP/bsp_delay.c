/*******************************************
	*�ļ��� ��  bsp_delay.c
	*��   �ߣ�  ����
	*�޸�ʱ�䣺 2021.02.20
	*��   ����  v1.0
  *˵   ����  ��ʱ��������Դ�ļ�
*******************************************/



#include "bsp_delay.h"


static u8  fac_us=0;							//us��ʱ�������ӣ�1us��Ӧsystickʱ������			   
static u16 fac_ms=0;							//ms��ʱ�������ӣ�1ms��Ӧsystickʱ������
	



/*
	*�������� Delay_Init()
	*��  �ܣ� ��ʱ��ʼ������
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.02.20
*/
void Delay_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	 //ѡ���ⲿʱ��  HCLK/8��9MHZ��
	fac_us=SystemCoreClock/8000000;				             //Ϊϵͳʱ�ӵ�1/8  
	fac_ms=(u16)fac_us*1000;					             //��OS��,����ÿ��ms��Ҫ��systickʱ����   
}



/*
	*�������� Delay_Ms()
	*��  �ܣ� ��ʱms����
    *��  ���� nms:��ʱms��
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.02.20
*/
void Delay_Ms(u16 nms)
{
    u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;							//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;                     //ѭ����ȡSysTick���ƼĴ�����ֵ
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;       					//��ռ�����
}


/*
	*�������� Delay_Us()
	*��  �ܣ� ��ʱus����
    *��  ���� nus:��ʱus��
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.02.20
*/
void Delay_Us(u32 nus)
{
    u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					 //ʱ�����	  		 
	SysTick->VAL=0x00;        					 //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	 //��ʼ����	  
	do
	{
		temp=SysTick->CTRL;                      //ѭ����ȡSysTick���ƼĴ�����ֵ
	}while((temp&0x01)&&!(temp&(1<<16)));		 //�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	 //�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����
}








































