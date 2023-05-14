/*******************************************
	*�ļ��� ��  TaskManage.c
	*��   �ߣ�  WF
	*�޸�ʱ�䣺 2021.03.22
	*��   ����  v1.0
  *˵   ����  �������ģ��Դ�ļ�
*******************************************/


#include "TaskManage.h"


TaskManage_t stTaskManager; //����һ�����������

/*
	*�������� Task_Manager_Init()
	*��  �ܣ� �����������ʼ������
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.22
*/
void Task_Manager_Init(void)
{
	u8 i = 0;
	
	/* ��������TASK[]����ʼ����������� */
	for(i = 0; i < TASK_NUM_MAX; i++)
	{
		stTaskManager.Task[i].Status = TASK_STOP;
		stTaskManager.Task[i].Time = 0;
		stTaskManager.Task[i].Count_Time = 0;
		stTaskManager.Task[i].Fuc = NULL;
	}
	stTaskManager.Regsiter_Task_Num = 0;//������ע�����������
}


/*
	*�������� Task_Regsiter()
	*��  �ܣ� �������������ע��һ������
	*��  ���� u8 index������������u8 time��������������, void* fuc������ص�����
	*����ֵ�� TURE��ע��ɹ���FALSE��ע��ʧ��
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.22
*/
u8 Task_Regsiter(u8 index, u8 time, void* fuc)
{
	/* �������������������򷵻�ע��ʧ�� */
	if(stTaskManager.Regsiter_Task_Num >= TASK_NUM_MAX)
	{
		return FALSE;
	}
	/* �������ص�����Ϊ�գ��򷵻�ע��ʧ�� */
	if(fuc == NULL)
	{
		return FALSE;
	}
	/* �����������δ��ע�ᣬ����Խ���ע�� */
	if(stTaskManager.Task[index].Fuc == NULL)
	{
		stTaskManager.Task[index].Status = TASK_STOP;
		stTaskManager.Task[index].Time = time;
		stTaskManager.Task[index].Count_Time = 0;
		stTaskManager.Task[index].Fuc = fuc;
		
		stTaskManager.Regsiter_Task_Num++; //��ע�������������1
		return TURE;//ע��ɹ�
	}
	
	return FALSE;
}


/*
	*�������� Task_Cancel()
	*��  �ܣ� ע������
	*��  ���� u8 index����������
  *����ֵ�� TURE��ע���ɹ���FALSE��ע��ʧ��
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.22
*/
u8 Task_Cancel(u8 index)
{
	/* �����ע��������Ϊ0���򷵻�ע��ʧ�� */
	if(stTaskManager.Regsiter_Task_Num == 0)
	{
		return FALSE;
	}
	/* �������ص�������Ϊ�գ������ע������ */
	if(stTaskManager.Task[index].Fuc != NULL)
	{
		stTaskManager.Task[index].Status = TASK_STOP;
		stTaskManager.Task[index].Time = 0;
		stTaskManager.Task[index].Count_Time = 0;
		stTaskManager.Task[index].Fuc = NULL;
		
		stTaskManager.Regsiter_Task_Num--; //��ע����������1
		return TURE; //ע��ʧ��
	}
	
	return FALSE;
}


/*
	*�������� Task_RunCheck()
	*��  �ܣ� �������������˶�
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.22
*/
void Task_RunCheck(void)
{
	u8 i = 0;
	
	for(i = 0; i < TASK_NUM_MAX; i++)
	{
		/* ��������Ѿ���ע�� */
		if(stTaskManager.Task[i].Fuc != NULL)
		{
			/* ������м��������ڵ����������� */
			if(stTaskManager.Task[i].Count_Time >= stTaskManager.Task[i].Time)
			{
				stTaskManager.Task[i].Count_Time = 0;
				stTaskManager.Task[i].Status = TASK_RUN; //�����������̬
			}
		}
	}
}


/*
	*�������� Task_Process()
	*��  �ܣ� ������
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.22
*/
void Task_Process(void)
{
	u8 i = 0;
	
	for(i = 0; i < TASK_NUM_MAX; i++)
	{
		/* ��������Ѿ���ע�� */
		if(stTaskManager.Task[i].Fuc != NULL)
		{
			/* �������������̬ */
			if(stTaskManager.Task[i].Status == TASK_RUN)
			{
				stTaskManager.Task[i].Fuc();
				stTaskManager.Task[i].Status = TASK_STOP;
			}
		}
	}
}

