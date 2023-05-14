/*******************************************
	*�ļ��� ��  bsp_delay.h
	*��   �ߣ�  ����
	*�޸�ʱ�䣺 2021.02.20
	*��   ����  v1.0
  *˵   ����  ��ʱ��������ͷ�ļ�
*******************************************/

/*
    SysTick��ʱ������ϸ���Ͽɲ鿴Cortex-M3Ȩ��ָ�ϣ����ģ�
    
    SysTick->CTRL���ƼĴ�����Ҫλ˵����
    bit0��SysTick ��ʱ����ʹ��λ
    
    bit1��SysTick ��ʱ�� SysTick �쳣���жϣ�����ʹ��λ��1��ENABLE, 0��DISABLE����
          ENABLE�����£�SysTick ������ 0 ʱ���� SysTick �쳣����
    
    bit16��������ϴζ�ȡ���Ĵ����� SysTick �Ѿ�������0�����λΪ 1��
           ͨ����ȡbit16�����жϼ����Ƿ�����������ȡ��λ����λ���Զ����㡣
    		    								   


    ע��nms�ķ�Χ
    SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
    nms<=0xffffff*8*1000/SYSCLK
    SYSCLK��λΪHz,nms��λΪms
    ��72M������,nms<=1864 
    
    ע�⣺��ʱ������ʵ�֣�û��ʹ��SysTick ��ʱ�� SysTick �쳣���жϣ���ʵ�֣�����ͨ�����϶�ȡbit16���ж�
          �����Ƿ�������Ͼ�us�����ж�̫����CPU�ˡ�
*/



#ifndef _BSP_DELAY_H
#define _BSP_DELAY_H 			   
#include "stm32f10x.h"  

	 
void Delay_Init(void);
void Delay_Ms(u16 nms);
void Delay_Us(u32 nus);

#endif


