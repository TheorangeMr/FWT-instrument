/*******************************************
	*�ļ��� ��  TaskManage.h
	*��   �ߣ�  WF
	*�޸�ʱ�䣺 2021.03.22
	*��   ����  v1.0
  *˵   ����  �������ģ��ͷ�ļ�
*******************************************/

#ifndef _TASKMANAGE_H
#define _TASKMANAGE_H


#include "stm32f10x.h"


/*****************************************************************
						          �������ͺ궨��
*****************************************************************/
#define TURE  (1)
#define FALSE (0)


#ifndef NULL 
#define NULL ((void*)(0))
#endif

/* ����״̬ */
#define TASK_STOP (0)  //���񲻿�����
#define TASK_RUN  (1)  //�����������

#define TASK_NUM_MAX (10) //���������

/*****************************************************************
										   �������Ͷ���
******************************************************************/
/* ����ṹ�� */
typedef struct
{
	u8 Status;            //��������״̬
	u8 Time;              //������������
	u8 Count_Time;        //�������м�����
	void(*Fuc)(void);     //�������к���ָ�루�ص�������
	//void* Fuc;
	
}TaskData_t;

/* �������ṹ�� */
typedef struct 
{
	TaskData_t Task[TASK_NUM_MAX];        //��������
	u8 Regsiter_Task_Num;                 //��ע�����������
}TaskManage_t;


/*****************************************************************
								         �������ӿ�
******************************************************************/
void Task_Manager_Init(void);
u8 Task_Regsiter(u8 index, u8 time, void* fuc);
u8 Task_Cancel(u8 index);
void Task_RunCheck(void);
void Task_Process(void);

	



#endif /* !_TASKMANAGE_H */

