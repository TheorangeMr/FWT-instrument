/*******************************************
	*�ļ��� ��  bsp_timer.c
	*��   �ߣ�  WF
	*�޸�ʱ�䣺 2021.04.22
	*��   ����  v1.0
  *˵   ����  ��ʱ��ģ��Դ�ļ�
*******************************************/


#include "bsp_timer.h"


#if TIM1_BASE_MODE

/* 
	*��������Timer1_Init()
	*��  �ܣ�TIMER1��ʼ������
	*��  �ߣ�WF
	*��  ����u16 psc:Ԥ��Ƶϵ��, u16 arr���Զ���װ��ֵ
	*����ֵ����
	*ʱ  �䣺2021.04.20
*/

void Timer1_Init(u16 psc, u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);            //ʹ�ܶ�ʱ��1ʱ��(ע��TIM1�ǹ���APB2�ϵ�����)
	
	/* ���ö�ʱ��1 */
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//����Ϊ���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //����Ƶ
	TIM_TimeBaseInitStructure.TIM_Period = arr;                    //�Զ�װ�س�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                 //Ԥ��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;           //�߼���ʱ����������
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);             //��ʼ����ʱ��
	
	/* ���ö�ʱ��1�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;             //��ʱ��1�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);                  //ʹ�ܸ����ж�ǰ��������жϱ�־��
                                                                 //��ֹһ��ʼʹ�ܾͽ�������ж�
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);                     //������ʱ��1�����ж�
	TIM_Cmd(TIM1, ENABLE);                                         //ʹ�ܶ�ʱ��
}

#endif

#if TIM1_INPUT_CAPTURE_MODE

/* 
	*��������Timer1_Init()
	*��  �ܣ�TIMER1��ʼ������
	*��  �ߣ�WF
	*��  ����u16 psc:Ԥ��Ƶϵ��, u16 arr���Զ���װ��ֵ
	*����ֵ����
	*ʱ  �䣺2021.04.20
*/

void Timer1_Init(u16 psc, u16 arr)
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	 /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* TIM1 channel 1 pin (PA.08) configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                     //�½��ش�������Ϊ��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ���ö�ʱ��1 */
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   //����Ϊ���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //����Ƶ
	TIM_TimeBaseInitStructure.TIM_Period = arr;                       //�Զ�װ�س�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                    //Ԥ��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;              //�߼���ʱ����������
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);                //��ʼ����ʱ��
	
	/* ���ö�ʱ��1���벶�� */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                  //ͨ��1
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;      //�½��ش���
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   //ֱ��ӳ��(TI1-->IC1)
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;             //����Ƶ
  TIM_ICInitStructure.TIM_ICFilter = 0x8;                           //�����˲�
	TIM_ICInit(TIM1, &TIM_ICInitStructure);

  /* Enable the TIM1 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;                //��ʱ��1ͨ��1����/�Ƚ��ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update|TIM_IT_CC1);          //ʹ�ܸ����ж�ǰ����жϱ�־��
                                                                    //��ֹһ��ʼʹ�ܾͽ����ж�
  /* TIM enable counter */
  TIM_Cmd(TIM1, ENABLE);
  /* Enable the CC1 and Update Interrupt Request */
  TIM_ITConfig(TIM1, TIM_IT_CC1|TIM_IT_Update, ENABLE);
}

#endif


#if TIM1_PWM_INPUT_MODE


/* 
	*��������Timer1_Init()
	*��  �ܣ�TIMER1��ʼ������
	*��  �ߣ�WF
	*��  ����u16 psc:Ԥ��Ƶϵ��, u16 arr���Զ���װ��ֵ
	*����ֵ����
	*ʱ  �䣺2021.04.20
*/
void Timer1_Init(u16 psc, u16 arr)
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* TIM1 channel 1 pin (PA.08) configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ���ö�ʱ��1 */
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   //����Ϊ���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //����Ƶ
	TIM_TimeBaseInitStructure.TIM_Period = arr;                       //�Զ�װ�س�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                    //Ԥ��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;              //�߼���ʱ����������
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);                //��ʼ����ʱ��

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x8;
  TIM_PWMIConfig(TIM1, &TIM_ICInitStructure);

  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update|TIM_IT_CC1);          //ʹ�ܸ����ж�ǰ����жϱ�־��
                                                                    //��ֹһ��ʼʹ�ܾͽ����ж�

  /* Select the TIM1 Input Trigger: TI1FP1 */
  TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1);
	
  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);

  /* Enable the Master/Slave Mode */
  TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);

  /* TIM enable counter */
  TIM_Cmd(TIM1, ENABLE);

  /* Enable the CC1 Interrupt Request */
  TIM_ITConfig(TIM1, TIM_IT_CC1|TIM_IT_Update, ENABLE);
}

#endif

#if TIM1_PWM_OUT

static void RCC_Configuration(void)
{
	/* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
}

/* TIM1_CH1-->PA8, TIM_CH2-->PA9, TIM1_CH3-->PA10, TIM1_CH4-->PA11 */
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIOA Configuration:TIM1 Channel1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Timer1_Init(u16 psc, u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_Configuration();     //PWM���ʱ������
	GPIO_Configuration();    //PWM�����������
	
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = arr;
  TIM_TimeBaseStructure.TIM_Prescaler = psc;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = arr/2;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = arr/2;

  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = arr/2;

  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = arr/2;

  TIM_OC4Init(TIM1, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM1, ENABLE);

  /* TIM1 enable counter */
  TIM_Cmd(TIM1, ENABLE);
}


void Timer1_Pwm_Set(u8 channel_x, u32 freq, u8 duty)
{
	u16 arr = 0;                       //ARR
	u16 crr = 0;                       //CRR
	
	/* ����Ԥ��Ƶϵ��Ϊ72-1 */
	arr = 1000000 / freq;
	TIM_SetAutoreload(TIM1, arr);      //�����Զ���װ��ֵ��������Ƶ��
	
	crr = (arr*duty) / 100;
	
	/* ѡ������ͨ�� */
	switch(channel_x)
	{
		case 1:
			TIM_SetCompare1(TIM1, crr);    //����CRR�Ĵ�����ֵ��������ռ�ձ�
			break;
		
		case 2:
			TIM_SetCompare2(TIM1, crr);
			break;
		
		case 3:
			TIM_SetCompare3(TIM1, crr);
			break;
		
		case 4:
			TIM_SetCompare4(TIM1, crr);
			break;
		
		default:
			break;
	}
}

#endif


#if TIM1_PWM_INPUT_MODE

/*
Capture_Status����״̬��
bit7:������ɱ�־λ
bit6:�״θߵ�ƽ�����־λ
bit5:�½��ز����־λ
bit4:����
bit[3:0]:��������� 
*/
/*
//����һ��
vu8 Capture_Status = 0;
vu8 High_Time_Over_Count = 0;
vu32 High_Time = 0;
vu32 Total_Time = 0;

void TIM1_CC_IRQHandler(void)
{
	if((TIM1->SR & TIM_IT_CC1) != 0)         //ͨ��1����
	{
		TIM1->SR = (uint16_t)~TIM_IT_CC1;      //����жϱ�־
		
		if((Capture_Status&0x40) != 0)         //����ڶ�������
		{
			Capture_Status |= 0x80;              //������ڲ������
			Total_Time = TIM1->CCR1;             //��ȡ��������
		}
		else                                   //�״β����Ͻ���
		{
			Capture_Status |= 0x40;              //����״β���������
		}
	}
	else if((TIM1->SR & TIM_IT_CC2) != 0)     //ͨ��2����
	{
		TIM1->SR  = (uint16_t)~TIM_IT_CC2;      //����жϱ�־
		High_Time = TIM1->CCR2;                 //��ȡ�ߵ�ƽʱ��
		Capture_Status |= 0x20;                 //����½��ز���
	}
}

void TIM1_UP_IRQHandler(void)
{
	TIM1->SR = (uint16_t)~TIM_IT_Update;      //����жϱ�־
	
	if((Capture_Status&0x20) == 0)            //����½��ر�־λû�б���λ
	{
		High_Time_Over_Count++;                 //�ߵ�ƽ�������
	}
	
	if((Capture_Status&0x80) == 0)            //���������ɱ�־û�б���λ
	{
		Capture_Status++;                       //�������
	}
	else
	{
		TIM1->DIER &= (uint16_t)~TIM_IT_Update; //��ֹ�����ж�
	}
}
*/


//��������
vu8 Capture_Flag = 0;
vu32 High_Time = 0;
vu32 Total_Time = 0;
void TIM1_CC_IRQHandler(void)
{
	/* ����жϱ�־ */
  TIM1->SR  = (uint16_t)~TIM_IT_CC1;      
  /* ��ȡ�������� */
  Total_Time = TIM1->CCR1;

  if (Total_Time != 0)
  {
    /* ��ȡ�ߵ�ƽʱ�� */
    High_Time = TIM1->CCR2;
		/* ��ǲ������ */
		Capture_Flag = 1;
  }
}

void TIM1_UP_IRQHandler(void)
{
	
}

#endif

#if TIM1_INPUT_CAPTURE_MODE

/*
Capture_Status����״̬��
bit7:������ɱ�־λ
bit6:�״θߵ�ƽ�����־λ
bit[5:0]:��������� 
*/
vu8 Capture_Status = 0;
vu32 Total_Time = 0;

/* ��ʱ��1����/�Ƚ��жϷ����� */
void TIM1_CC_IRQHandler(void)
{
	if((TIM1->SR & TIM_IT_CC1) != 0)            //ͨ��1����
	{
		TIM1->SR = (uint16_t)~TIM_IT_CC1;         //����жϱ�־
		if((Capture_Status&0x40) != 0)            //����ڶ���������
		{
			Total_Time = TIM1->CNT;                 //��ȡ�������� 
			TIM1->DIER &= (uint16_t)~TIM_IT_CC1;    //��ֹ����/�Ƚ�1�ж�
			Capture_Status |= 0x80;                 //��ǲ������
		}
		else
		{
			TIM1->CNT = 0;                          //����������
			Capture_Status |= 0x40;                 //����״β���������
		}
	}
	
}

/* ��ʱ��1�����жϷ����� */
void TIM1_UP_IRQHandler(void)
{
	TIM1->SR = (uint16_t)~TIM_IT_Update;        //����жϱ�־;
	if((Capture_Status & 0x80) == 0)            //���������ɱ�־û�б���λ
	{
		Capture_Status++;                         //�������
	}
	else
	{
		TIM1->DIER &= (uint16_t)~TIM_IT_Update;   //��ֹ�����ж�
	}
}

#endif


#if TIM1_BASE_MODE

/* ��ʱ��1�����жϷ����� */
void TIM1_UP_IRQHandler(void)
{
	TIM1->SR = (uint16_t)~TIM_IT_Update;     //����жϱ�־
}

#endif

#if TIM2_BASE_MODE

/* 
	*��������Timer2_Init()
	*��  �ܣ�TIMER2��ʼ������
	*��  �ߣ�WF
	*��  ����u16 psc:Ԥ��Ƶϵ��, u16 arr���Զ���װ��ֵ
	*����ֵ����
	*ʱ  �䣺2021.04.20
*/

void Timer2_Init(u16 psc, u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);            //ʹ�ܶ�ʱ��2ʱ��
	
	/* ���ö�ʱ��2 */
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//����Ϊ���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //����Ƶ
	TIM_TimeBaseInitStructure.TIM_Period = arr;                    //�Զ�װ�س�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                 //Ԥ��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;           //�߼���ʱ����������
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);            //��ʼ����ʱ��
	
	/* ���ö�ʱ��2�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;                //��ʱ��2�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);                  //ʹ�ܸ����ж�ǰ��������жϱ�־��
                                                                 //��ֹһ��ʼʹ�ܾͽ�������ж�
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                     //������ʱ��2�����ж�
	TIM_Cmd(TIM2, ENABLE);                                         //ʹ�ܶ�ʱ��
}


#endif 


#if TIM3_BASE_MODE

/* 
	*��������Timer3_Init()
	*��  �ܣ�TIMER3��ʼ������
	*��  �ߣ�WF
	*��  ����u16 psc:Ԥ��Ƶϵ��, u16 arr���Զ���װ��ֵ
	*����ֵ����
	*ʱ  �䣺2021.06.02
*/

void Timer3_Init(u16 psc, u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);            //ʹ�ܶ�ʱ��3ʱ��
	
	/* ���ö�ʱ��3 */
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//����Ϊ���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //����Ƶ
	TIM_TimeBaseInitStructure.TIM_Period = arr;                    //�Զ�װ�س�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                 //Ԥ��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;           //�߼���ʱ����������
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);            //��ʼ����ʱ��
	
	/* ���ö�ʱ��3�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;                //��ʱ��3�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);                  //ʹ�ܸ����ж�ǰ��������жϱ�־��
                                                                 //��ֹһ��ʼʹ�ܾͽ�������ж�
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                     //������ʱ��3�����ж�
	TIM_Cmd(TIM3, ENABLE);                                         //ʹ�ܶ�ʱ��
}


#endif 



