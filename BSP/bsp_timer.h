/*******************************************
	*�ļ��� ��  bsp_timer.h
	*��   �ߣ�  WF
	*�޸�ʱ�䣺 2021.04.22
	*��   ����  v1.0
  *˵   ����  ��ʱ��ģ��ͷ�ļ�
*******************************************/



#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

#include "stm32f10x.h"



/*******************************************************************************************
																�߼���ʱ����TIM1��TIM8��
*******************************************************************************************/
/* TIM1����ģʽ�궨�� */
#define TIM1_PWM_INPUT_MODE       0     //��ʱ��1��PWM����ģʽ
#define TIM1_INPUT_CAPTURE_MODE   0     //��ʱ��1�����벶��ģʽ
#define TIM1_BASE_MODE            0     //��ʱ��1��������ʱģʽ
#define TIM1_PWM_OUT              1     //��ʱ��1��PWM���ģʽ
/* TIM8����ģʽ�궨�� */
#define TIM8_PWM_INPUT_MODE       0     //��ʱ��1��PWM����ģʽ
#define TIM8_INPUT_CAPTURE_MODE   0     //��ʱ��1�����벶��ģʽ
#define TIM8_BASE_MODE            0     //��ʱ��1��������ʱģʽ

/******************************************************************************************
                                ͨ�ö�ʱ����TIM2��3��4��5��
******************************************************************************************/
/* TIM2����ģʽ�궨�� */
#define TIM2_PWM_INPUT_MODE       0     //��ʱ��1��PWM����ģʽ
#define TIM2_INPUT_CAPTURE_MODE   0     //��ʱ��1�����벶��ģʽ
#define TIM2_BASE_MODE            1     //��ʱ��1��������ʱģʽ
/* TIM3����ģʽ�궨�� */
#define TIM3_PWM_INPUT_MODE       0     //��ʱ��1��PWM����ģʽ
#define TIM3_INPUT_CAPTURE_MODE   0     //��ʱ��1�����벶��ģʽ
#define TIM3_BASE_MODE            1     //��ʱ��1��������ʱģʽ
/* TIM4����ģʽ�궨�� */
#define TIM4_PWM_INPUT_MODE       0     //��ʱ��1��PWM����ģʽ
#define TIM4_INPUT_CAPTURE_MODE   0     //��ʱ��1�����벶��ģʽ
#define TIM4_BASE_MODE            0     //��ʱ��1��������ʱģʽ
/* TIM5����ģʽ�궨�� */
#define TIM5_PWM_INPUT_MODE       0     //��ʱ��1��PWM����ģʽ
#define TIM5_INPUT_CAPTURE_MODE   0     //��ʱ��1�����벶��ģʽ
#define TIM5_BASE_MODE            0     //��ʱ��1��������ʱģʽ


/*****************************************************************************************
																��ʱ�������ӿ�
*****************************************************************************************/
void Timer1_Init(u16 psc, u16 arr);
void Timer2_Init(u16 psc, u16 arr);
void Timer3_Init(u16 psc, u16 arr);
void Timer4_Init(u16 psc, u16 arr);
void Timer5_Init(u16 psc, u16 arr);
void Timer6_Init(u16 psc, u16 arr);
void Timer7_Init(u16 psc, u16 arr);
void Timer8_Init(u16 psc, u16 arr);
void Timer1_Pwm_Set(u8 channel_x, u32 freq, u8 duty);





#endif /* !_BSP_TIMER_H */


