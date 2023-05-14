/*******************************************
	*文件名 ：  TaskManage.c
	*作   者：  WF
	*修改时间： 2021.03.22
	*版   本：  v1.0
  *说   明：  任务管理模块源文件
*******************************************/


#include "TaskManage.h"


TaskManage_t stTaskManager; //定义一个任务管理器

/*
	*函数名： Task_Manager_Init()
	*功  能： 任务管理器初始化函数
	*参  数： 无
	*返回值： 无
	*作  者： WF
	*日  期： 2021.03.22
*/
void Task_Manager_Init(void)
{
	u8 i = 0;
	
	/* 遍历数组TASK[]，初始化任务管理器 */
	for(i = 0; i < TASK_NUM_MAX; i++)
	{
		stTaskManager.Task[i].Status = TASK_STOP;
		stTaskManager.Task[i].Time = 0;
		stTaskManager.Task[i].Count_Time = 0;
		stTaskManager.Task[i].Fuc = NULL;
	}
	stTaskManager.Regsiter_Task_Num = 0;//清零已注册任务计数器
}


/*
	*函数名： Task_Regsiter()
	*功  能： 在任务管理器中注册一个任务
	*参  数： u8 index：任务索引，u8 time：任务运行周期, void* fuc：任务回调函数
	*返回值： TURE：注册成功，FALSE：注册失败
	*作  者： WF
	*日  期： 2021.03.22
*/
u8 Task_Regsiter(u8 index, u8 time, void* fuc)
{
	/* 如果任务管理器已满，则返回注册失败 */
	if(stTaskManager.Regsiter_Task_Num >= TASK_NUM_MAX)
	{
		return FALSE;
	}
	/* 如果任务回调函数为空，则返回注册失败 */
	if(fuc == NULL)
	{
		return FALSE;
	}
	/* 如果任务索引未被注册，则可以进行注册 */
	if(stTaskManager.Task[index].Fuc == NULL)
	{
		stTaskManager.Task[index].Status = TASK_STOP;
		stTaskManager.Task[index].Time = time;
		stTaskManager.Task[index].Count_Time = 0;
		stTaskManager.Task[index].Fuc = fuc;
		
		stTaskManager.Regsiter_Task_Num++; //已注册任务计数器加1
		return TURE;//注册成功
	}
	
	return FALSE;
}


/*
	*函数名： Task_Cancel()
	*功  能： 注销任务
	*参  数： u8 index：任务索引
  *返回值： TURE：注销成功，FALSE：注销失败
	*作  者： WF
	*日  期： 2021.03.22
*/
u8 Task_Cancel(u8 index)
{
	/* 如果已注册任务数为0，则返回注销失败 */
	if(stTaskManager.Regsiter_Task_Num == 0)
	{
		return FALSE;
	}
	/* 如果任务回调函数不为空，则进行注销操作 */
	if(stTaskManager.Task[index].Fuc != NULL)
	{
		stTaskManager.Task[index].Status = TASK_STOP;
		stTaskManager.Task[index].Time = 0;
		stTaskManager.Task[index].Count_Time = 0;
		stTaskManager.Task[index].Fuc = NULL;
		
		stTaskManager.Regsiter_Task_Num--; //已注册任务数减1
		return TURE; //注销失败
	}
	
	return FALSE;
}


/*
	*函数名： Task_RunCheck()
	*功  能： 任务运行条件核对
	*参  数： 无
	*返回值： 无
	*作  者： WF
	*日  期： 2021.03.22
*/
void Task_RunCheck(void)
{
	u8 i = 0;
	
	for(i = 0; i < TASK_NUM_MAX; i++)
	{
		/* 如果任务已经被注册 */
		if(stTaskManager.Task[i].Fuc != NULL)
		{
			/* 如果运行计数器大于等于任务周期 */
			if(stTaskManager.Task[i].Count_Time >= stTaskManager.Task[i].Time)
			{
				stTaskManager.Task[i].Count_Time = 0;
				stTaskManager.Task[i].Status = TASK_RUN; //任务进入运行态
			}
		}
	}
}


/*
	*函数名： Task_Process()
	*功  能： 任务处理
	*参  数： 无
	*返回值： 无
	*作  者： WF
	*日  期： 2021.03.22
*/
void Task_Process(void)
{
	u8 i = 0;
	
	for(i = 0; i < TASK_NUM_MAX; i++)
	{
		/* 如果任务已经被注册 */
		if(stTaskManager.Task[i].Fuc != NULL)
		{
			/* 如果任务处于运行态 */
			if(stTaskManager.Task[i].Status == TASK_RUN)
			{
				stTaskManager.Task[i].Fuc();
				stTaskManager.Task[i].Status = TASK_STOP;
			}
		}
	}
}

