/*******************************************
	*文件名 ：  iwdg.c
	*作   者：  王峰
	*修改时间： 2020.11.07
	*版   本：  v1.0
*******************************************/

#include "iwdg.h"



/*
	*函数名：Iwdg_Init()
	*功  能：独立看门狗初始化函数
	*参  数： prer：预分频系数， rlr：自动重装载值
	*返回值： 无
	*作  者： 王峰
	*日  期： 2020.10.24
*/
void Iwdg_Init(u8 prer, u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);        //写使能，取消写保护
	IWDG_SetPrescaler(prer);                             //设置预分频系数
	IWDG_SetReload(rlr);                                 //设置重装载值
	IWDG_ReloadCounter();                                //喂狗
	IWDG_Enable();                                       //独立看门狗使能
}

