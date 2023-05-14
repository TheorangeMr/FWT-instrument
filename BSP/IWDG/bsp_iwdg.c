/*******************************************
	*�ļ��� ��  bsp_iwdg.c
	*��   �ߣ�  �޳�
	*�޸�ʱ�䣺 2022.03.13
	*��   ����  v1.0
  *˵   ����  IWDG�ļ�
*******************************************/

#include "bsp_iwdg.h"

void WWDG_Config(uint8_t prer,uint16_t rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prer);
	IWDG_SetReload(rlr);
	IWDG_ReloadCounter();
	IWDG_Enable();
}
