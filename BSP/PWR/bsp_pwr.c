/*******************************************
	*�ļ��� ��  bsp_pwr.c
	*��   �ߣ�  �޳�
	*�޸�ʱ�䣺 2022.06.10
	*��   ����  v1.0
*******************************************/

#include "bsp_pwr.h"


/*˯��ģʽ*/
void Sleep_mode(void)
{
	__WFI();
}

/*ֹͣģʽ*/
void Stop_mode(void)
{
//	DBGMCU_Config(DBGMCU_IWDG_STOP,ENABLE);      /*��Ч*/
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

void SYSCLKConfig_STOP(void)
{
	  SystemInit();
}

void Stop_interface(void)
{
	Stop_mode();
	SYSCLKConfig_STOP();
}

/* ����ģʽ */
void Standby_mode(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
	/*���WU״̬λ*/
	PWR_ClearFlag(PWR_FLAG_WU);
//	/*ʹ��WKUP���ŵĻ��ѹ��ܣ�ʹ��PA0*/
//	PWR_WakeUpPinCmd(ENABLE);
  /* �������ģʽ */	
	PWR_EnterSTANDBYMode();
}


