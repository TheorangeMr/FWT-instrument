/*******************************************
	*文件名 ：  iwdg.h
	*作   者：  王峰
	*修改时间： 2020.11.07
	*版   本：  v1.0
*******************************************/

#ifndef _IWDG_H
#define _IWDG_H

#include "sys.h"

void Iwdg_Init(u8 prer, u16 rlr);

/**************************独立看门狗溢出时间计算公式************************/ 

// Tout = ((4 * 2^prer) * rlr) / 40 (ms)


#endif

