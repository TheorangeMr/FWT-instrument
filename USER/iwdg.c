/*******************************************
	*�ļ��� ��  iwdg.c
	*��   �ߣ�  ����
	*�޸�ʱ�䣺 2020.11.07
	*��   ����  v1.0
*******************************************/

#include "iwdg.h"



/*
	*��������Iwdg_Init()
	*��  �ܣ��������Ź���ʼ������
	*��  ���� prer��Ԥ��Ƶϵ���� rlr���Զ���װ��ֵ
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2020.10.24
*/
void Iwdg_Init(u8 prer, u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);        //дʹ�ܣ�ȡ��д����
	IWDG_SetPrescaler(prer);                             //����Ԥ��Ƶϵ��
	IWDG_SetReload(rlr);                                 //������װ��ֵ
	IWDG_ReloadCounter();                                //ι��
	IWDG_Enable();                                       //�������Ź�ʹ��
}

