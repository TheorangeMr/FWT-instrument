/*******************************************
	*文件名 ：  TaskManage.h
	*作   者：  WF
	*修改时间： 2021.03.22
	*版   本：  v1.0
  *说   明：  任务管理模块头文件
*******************************************/

#ifndef _TASKMANAGE_H
#define _TASKMANAGE_H


#include "stm32f10x.h"


/*****************************************************************
						          数据类型宏定义
*****************************************************************/
#define TURE  (1)
#define FALSE (0)


#ifndef NULL 
#define NULL ((void*)(0))
#endif

/* 运行状态 */
#define TASK_STOP (0)  //任务不可运行
#define TASK_RUN  (1)  //任务可以运行

#define TASK_NUM_MAX (10) //最大任务数

/*****************************************************************
										   任务类型定义
******************************************************************/
/* 任务结构体 */
typedef struct
{
	u8 Status;            //任务运行状态
	u8 Time;              //任务运行周期
	u8 Count_Time;        //任务运行计数器
	void(*Fuc)(void);     //任务运行函数指针（回调函数）
	//void* Fuc;
	
}TaskData_t;

/* 任务管理结构体 */
typedef struct 
{
	TaskData_t Task[TASK_NUM_MAX];        //任务数组
	u8 Regsiter_Task_Num;                 //已注册任务计数器
}TaskManage_t;


/*****************************************************************
								         任务函数接口
******************************************************************/
void Task_Manager_Init(void);
u8 Task_Regsiter(u8 index, u8 time, void* fuc);
u8 Task_Cancel(u8 index);
void Task_RunCheck(void);
void Task_Process(void);

	



#endif /* !_TASKMANAGE_H */

