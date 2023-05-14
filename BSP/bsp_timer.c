/*******************************************
	*文件名 ：  bsp_timer.c
	*作   者：  WF
	*修改时间： 2021.04.22
	*版   本：  v1.0
  *说   明：  定时器模块源文件
*******************************************/


#include "bsp_timer.h"


#if TIM1_BASE_MODE

/* 
	*函数名：Timer1_Init()
	*功  能：TIMER1初始化函数
	*作  者：WF
	*参  数：u16 psc:预分频系数, u16 arr：自动重装载值
	*返回值：无
	*时  间：2021.04.20
*/

void Timer1_Init(u16 psc, u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);            //使能定时器1时钟(注意TIM1是挂载APB2上的外设)
	
	/* 配置定时器1 */
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//设置为向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //不分频
	TIM_TimeBaseInitStructure.TIM_Period = arr;                    //自动装载初值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                 //预分频系数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;           //高级定时器必须配置
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);             //初始化定时器
	
	/* 配置定时器1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;             //定时器1更新中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);                  //使能更新中断前清除更新中断标志，
                                                                 //防止一开始使能就进入更新中断
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);                     //开启定时器1更新中断
	TIM_Cmd(TIM1, ENABLE);                                         //使能定时器
}

#endif

#if TIM1_INPUT_CAPTURE_MODE

/* 
	*函数名：Timer1_Init()
	*功  能：TIMER1初始化函数
	*作  者：WF
	*参  数：u16 psc:预分频系数, u16 arr：自动重装载值
	*返回值：无
	*时  间：2021.04.20
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                     //下降沿触发设置为上拉输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 配置定时器1 */
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   //设置为向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //不分频
	TIM_TimeBaseInitStructure.TIM_Period = arr;                       //自动装载初值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                    //预分频系数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;              //高级定时器必须配置
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);                //初始化定时器
	
	/* 配置定时器1输入捕获 */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                  //通道1
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;      //下降沿触发
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   //直接映射(TI1-->IC1)
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;             //不分频
  TIM_ICInitStructure.TIM_ICFilter = 0x8;                           //输入滤波
	TIM_ICInit(TIM1, &TIM_ICInitStructure);

  /* Enable the TIM1 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;                //定时器1通道1捕获/比较中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update|TIM_IT_CC1);          //使能更新中断前清除中断标志，
                                                                    //防止一开始使能就进入中断
  /* TIM enable counter */
  TIM_Cmd(TIM1, ENABLE);
  /* Enable the CC1 and Update Interrupt Request */
  TIM_ITConfig(TIM1, TIM_IT_CC1|TIM_IT_Update, ENABLE);
}

#endif


#if TIM1_PWM_INPUT_MODE


/* 
	*函数名：Timer1_Init()
	*功  能：TIMER1初始化函数
	*作  者：WF
	*参  数：u16 psc:预分频系数, u16 arr：自动重装载值
	*返回值：无
	*时  间：2021.04.20
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
	
	/* 配置定时器1 */
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   //设置为向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //不分频
	TIM_TimeBaseInitStructure.TIM_Period = arr;                       //自动装载初值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                    //预分频系数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;              //高级定时器必须配置
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);                //初始化定时器

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
	
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update|TIM_IT_CC1);          //使能更新中断前清除中断标志，
                                                                    //防止一开始使能就进入中断

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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Timer1_Init(u16 psc, u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_Configuration();     //PWM输出时钟配置
	GPIO_Configuration();    //PWM输出引脚配置
	
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
	
	/* 假设预分频系数为72-1 */
	arr = 1000000 / freq;
	TIM_SetAutoreload(TIM1, arr);      //设置自动重装载值就是设置频率
	
	crr = (arr*duty) / 100;
	
	/* 选择设置通道 */
	switch(channel_x)
	{
		case 1:
			TIM_SetCompare1(TIM1, crr);    //设置CRR寄存器的值就是设置占空比
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
Capture_Status捕获状态字
bit7:捕获完成标志位
bit6:首次高电平捕获标志位
bit5:下降沿捕获标志位
bit4:保留
bit[3:0]:溢出计数器 
*/
/*
//方法一：
vu8 Capture_Status = 0;
vu8 High_Time_Over_Count = 0;
vu32 High_Time = 0;
vu32 Total_Time = 0;

void TIM1_CC_IRQHandler(void)
{
	if((TIM1->SR & TIM_IT_CC1) != 0)         //通道1捕获
	{
		TIM1->SR = (uint16_t)~TIM_IT_CC1;      //清除中断标志
		
		if((Capture_Status&0x40) != 0)         //捕获第二次上升
		{
			Capture_Status |= 0x80;              //标记周期测量完成
			Total_Time = TIM1->CCR1;             //获取脉冲周期
		}
		else                                   //首次捕获上降沿
		{
			Capture_Status |= 0x40;              //标记首次捕获上升沿
		}
	}
	else if((TIM1->SR & TIM_IT_CC2) != 0)     //通道2捕获
	{
		TIM1->SR  = (uint16_t)~TIM_IT_CC2;      //清除中断标志
		High_Time = TIM1->CCR2;                 //获取高电平时间
		Capture_Status |= 0x20;                 //标记下降沿捕获
	}
}

void TIM1_UP_IRQHandler(void)
{
	TIM1->SR = (uint16_t)~TIM_IT_Update;      //清除中断标志
	
	if((Capture_Status&0x20) == 0)            //如果下降沿标志位没有被置位
	{
		High_Time_Over_Count++;                 //高电平溢出计数
	}
	
	if((Capture_Status&0x80) == 0)            //如果捕获完成标志没有被置位
	{
		Capture_Status++;                       //溢出计数
	}
	else
	{
		TIM1->DIER &= (uint16_t)~TIM_IT_Update; //禁止更新中断
	}
}
*/


//方法二：
vu8 Capture_Flag = 0;
vu32 High_Time = 0;
vu32 Total_Time = 0;
void TIM1_CC_IRQHandler(void)
{
	/* 清除中断标志 */
  TIM1->SR  = (uint16_t)~TIM_IT_CC1;      
  /* 获取脉冲周期 */
  Total_Time = TIM1->CCR1;

  if (Total_Time != 0)
  {
    /* 获取高电平时间 */
    High_Time = TIM1->CCR2;
		/* 标记捕获完成 */
		Capture_Flag = 1;
  }
}

void TIM1_UP_IRQHandler(void)
{
	
}

#endif

#if TIM1_INPUT_CAPTURE_MODE

/*
Capture_Status捕获状态字
bit7:捕获完成标志位
bit6:首次高电平捕获标志位
bit[5:0]:溢出计数器 
*/
vu8 Capture_Status = 0;
vu32 Total_Time = 0;

/* 定时器1捕获/比较中断服务函数 */
void TIM1_CC_IRQHandler(void)
{
	if((TIM1->SR & TIM_IT_CC1) != 0)            //通道1捕获
	{
		TIM1->SR = (uint16_t)~TIM_IT_CC1;         //清除中断标志
		if((Capture_Status&0x40) != 0)            //捕获第二次上升沿
		{
			Total_Time = TIM1->CNT;                 //获取脉冲周期 
			TIM1->DIER &= (uint16_t)~TIM_IT_CC1;    //禁止捕获/比较1中断
			Capture_Status |= 0x80;                 //标记捕获完成
		}
		else
		{
			TIM1->CNT = 0;                          //计数器清零
			Capture_Status |= 0x40;                 //标记首次捕获上升沿
		}
	}
	
}

/* 定时器1更新中断服务函数 */
void TIM1_UP_IRQHandler(void)
{
	TIM1->SR = (uint16_t)~TIM_IT_Update;        //清除中断标志;
	if((Capture_Status & 0x80) == 0)            //如果捕获完成标志没有被置位
	{
		Capture_Status++;                         //溢出计数
	}
	else
	{
		TIM1->DIER &= (uint16_t)~TIM_IT_Update;   //禁止更新中断
	}
}

#endif


#if TIM1_BASE_MODE

/* 定时器1更新中断服务函数 */
void TIM1_UP_IRQHandler(void)
{
	TIM1->SR = (uint16_t)~TIM_IT_Update;     //清除中断标志
}

#endif

#if TIM2_BASE_MODE

/* 
	*函数名：Timer2_Init()
	*功  能：TIMER2初始化函数
	*作  者：WF
	*参  数：u16 psc:预分频系数, u16 arr：自动重装载值
	*返回值：无
	*时  间：2021.04.20
*/

void Timer2_Init(u16 psc, u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);            //使能定时器2时钟
	
	/* 配置定时器2 */
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//设置为向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //不分频
	TIM_TimeBaseInitStructure.TIM_Period = arr;                    //自动装载初值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                 //预分频系数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;           //高级定时器必须配置
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);            //初始化定时器
	
	/* 配置定时器2中断 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;                //定时器2更新中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);                  //使能更新中断前清除更新中断标志，
                                                                 //防止一开始使能就进入更新中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                     //开启定时器2更新中断
	TIM_Cmd(TIM2, ENABLE);                                         //使能定时器
}


#endif 


#if TIM3_BASE_MODE

/* 
	*函数名：Timer3_Init()
	*功  能：TIMER3初始化函数
	*作  者：WF
	*参  数：u16 psc:预分频系数, u16 arr：自动重装载值
	*返回值：无
	*时  间：2021.06.02
*/

void Timer3_Init(u16 psc, u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);            //使能定时器3时钟
	
	/* 配置定时器3 */
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//设置为向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //不分频
	TIM_TimeBaseInitStructure.TIM_Period = arr;                    //自动装载初值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                 //预分频系数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;           //高级定时器必须配置
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);            //初始化定时器
	
	/* 配置定时器3中断 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;                //定时器3更新中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);                  //使能更新中断前清除更新中断标志，
                                                                 //防止一开始使能就进入更新中断
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                     //开启定时器3更新中断
	TIM_Cmd(TIM3, ENABLE);                                         //使能定时器
}


#endif 



