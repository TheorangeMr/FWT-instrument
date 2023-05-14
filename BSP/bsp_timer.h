/*******************************************
	*文件名 ：  bsp_timer.h
	*作   者：  WF
	*修改时间： 2021.04.22
	*版   本：  v1.0
  *说   明：  定时器模块头文件
*******************************************/



#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

#include "stm32f10x.h"



/*******************************************************************************************
																高级定时器（TIM1、TIM8）
*******************************************************************************************/
/* TIM1工作模式宏定义 */
#define TIM1_PWM_INPUT_MODE       0     //定时器1：PWM输入模式
#define TIM1_INPUT_CAPTURE_MODE   0     //定时器1：输入捕获模式
#define TIM1_BASE_MODE            0     //定时器1：基本定时模式
#define TIM1_PWM_OUT              1     //定时器1：PWM输出模式
/* TIM8工作模式宏定义 */
#define TIM8_PWM_INPUT_MODE       0     //定时器1：PWM输入模式
#define TIM8_INPUT_CAPTURE_MODE   0     //定时器1：输入捕获模式
#define TIM8_BASE_MODE            0     //定时器1：基本定时模式

/******************************************************************************************
                                通用定时器（TIM2、3、4、5）
******************************************************************************************/
/* TIM2工作模式宏定义 */
#define TIM2_PWM_INPUT_MODE       0     //定时器1：PWM输入模式
#define TIM2_INPUT_CAPTURE_MODE   0     //定时器1：输入捕获模式
#define TIM2_BASE_MODE            1     //定时器1：基本定时模式
/* TIM3工作模式宏定义 */
#define TIM3_PWM_INPUT_MODE       0     //定时器1：PWM输入模式
#define TIM3_INPUT_CAPTURE_MODE   0     //定时器1：输入捕获模式
#define TIM3_BASE_MODE            1     //定时器1：基本定时模式
/* TIM4工作模式宏定义 */
#define TIM4_PWM_INPUT_MODE       0     //定时器1：PWM输入模式
#define TIM4_INPUT_CAPTURE_MODE   0     //定时器1：输入捕获模式
#define TIM4_BASE_MODE            0     //定时器1：基本定时模式
/* TIM5工作模式宏定义 */
#define TIM5_PWM_INPUT_MODE       0     //定时器1：PWM输入模式
#define TIM5_INPUT_CAPTURE_MODE   0     //定时器1：输入捕获模式
#define TIM5_BASE_MODE            0     //定时器1：基本定时模式


/*****************************************************************************************
																定时器函数接口
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


