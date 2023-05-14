/*******************************************
	*文件名 ：  main.c
	*作   者：  WF
	*修改时间： 2022.05.10
	*版   本：  v2.0
  *说   明：  主函数源文件
*******************************************/


#include "stm32f10x.h"
#include "bsp_delay.h"
#include "bsp_usart.h"
#include "string.h"
#include "bsp_lcd7_0.h"
#include "bsp_can.h"
#include "bsp_timer.h"
#include "bsp_rtc.h"
#include "TaskManage.h"
#include "GUI.h"
#include "ff.h"
#include "ffconf.h"
#include "bsp_sdio_sdcard.h"
#include "bsp_ds18b20.h"
#include "iwdg.h"
#include "WM.h"
#include "PROGBAR.h"
#include "bsp_iwdg.h"
#include "bsp_pwr.h"



/******************************************************************************
															外部变量声明
*******************************************************************************/
extern CALENDAR Calendar;               //RTC日历
extern TaskManage_t stTaskManager;      //任务管理器
WM_HWIN hWnd;
/******************************************************************************
															外部函数声明
*******************************************************************************/
extern void _DemoBandingMemdev_Speed(u8 zdata);
extern void _DemoBandingMemdev_Oil( u8 zdata);
extern void _DemoBandingMemdev_Mileage( float zdata);
extern void _DemoBandingMemdev_Battery(float zdata);
extern void _DemoBandingMemdev_Network(float zdata);
void _BkcbWindow(WM_MESSAGE* pMsg);

/******************************************************************************
															本地函变量定义
*******************************************************************************/
FATFS fsp;                        //文件系统句柄
BYTE work[FF_MAX_SS] = {0};       //工作缓冲区
FIL flp;                          //文件句柄
FATFS *pfs;                       //指向文件系统的指针
FATFS *pfs;                       //指向文件系统的指针
uint8_t Display_Buf[20] = {0};    //液晶屏显示缓存数组
vu8 CAN_Recive_Flag = 0;          //CAN接收标志
vu8 VCU_Status1_Receive[8];       //VCU_Status1接收报文数组
vu8 VCU_Status2_Receive[8];       //VCU_Status2接收报文数组
u8 Connection_Flag = 0;           //通信连接标志
u8 Car_Speed = 0;                 //车速
u8 Car_Oil = 0;                   //油量
float Car_Mileage = 0;            //里程
u16 Temperature = 0;              //仪表温度
uint8_t Batter_flag = 4;          //电池电量格
uint8_t Network_signal = 1;       //4G信号
uint8_t RTC_Time_flag = 0;
CALENDAR Calendar;


uint8_t Standby_flag = 0;

extern uint16_t RTCvalue;

u8 wendudat[10] = {5};
//u8 Ds18b20_ID[10] = {0};

/******************************************************************************
															本地函数声明
*******************************************************************************/
void Bsp_Init(void);
void Task1_CallBack(void);
void Task2_CallBack(void);
void Task3_CallBack(void);
void Task4_CallBack(void);
int _Get_Data(void * p,const U8 ** ppData, unsigned NumBytesReq, U32 offset);


int main()
{
	/* 板级外设初始化 */
	Bsp_Init();
	/* STemWin初始化 */
	GUI_Init();
	
	hWnd = WM_CreateWindow(0,0,800,480,WM_CF_BGND,_BkcbWindow,0);
	
	Temperature = Ds18b20_ReadSingleTemp(0);          //读取温度  
	CAN_Config();                                     //配置CAN模块
	
//	Iwdg_Init(4, 625);                                //独立看门狗初始化，设置溢出时间1s
	
	Task_Manager_Init();                              //任务管理器初始化
	Task_Regsiter(0, 20, Task1_CallBack);             //注册任务1
	Task_Regsiter(1, 20, Task2_CallBack);             //注册任务2
	Task_Regsiter(2, 20, Task3_CallBack);             //注册任务3
	Task_Regsiter(3, 100, Task4_CallBack);            //注册任务4
	
	Timer2_Init(720-1, 1000-1);                       //定时器2初始化，产生任务节拍
	while(1)
	{
		Task_RunCheck(); //任务运行条件核对
		Task_Process();  //任务运行处理
	}
}



 /* 流位图数据获取函数 */
int _Get_Data(void * p,const U8 ** ppData,unsigned NumBytesReq,U32 offset )
{
	FRESULT res_SD = FR_OK;
	FIL * phFile = (FIL *)p;
	U8  * pData  = (U8 *)*ppData;
	DWORD NumBytesRead;
	if(1)
	{
		res_SD = f_open(phFile,"0:fwt.dta",FA_READ|FA_OPEN_EXISTING);
		f_lseek(phFile,offset);
		res_SD = f_read(phFile,pData,NumBytesReq,&NumBytesRead);			
		f_close(phFile);
	}
	return NumBytesRead; 
}



/* 板级外设初始化函数 */
void Bsp_Init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE); //使能CRC时钟
	Delay_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Usart1_Init(115200);
	RTC_Init();
  //W25QXX_Init();
  MD070SD_Init();
  Delay_Ms(500);
	wendudat[8] = Ds18b20_Init();
	Timer3_Init(7200-1, 5000-1);                      //定时器3初始化      500ms
}

/* 定时器2更新中断服务函数（任务时钟节拍计数） */
void TIM2_IRQHandler(void)
{
	TIM2->SR = (uint16_t)~TIM_IT_Update;              //清除中断标志
	
	u8 i = 0;
	
	for(i = 0; i < TASK_NUM_MAX; i++)
	{
		if(stTaskManager.Task[i].Fuc != NULL)
		{
			stTaskManager.Task[i].Count_Time++;
		}
	}
}

/* 定时器3更新中断服务函数（看门狗喂狗） */
void TIM3_IRQHandler(void)
{
	static uint32_t tim3_dat = 0;
	TIM3->SR = (uint16_t)~TIM_IT_Update;     //清除中断标志
	if(tim3_dat >= 6000)
	{
		Standby_flag = 1;
		tim3_dat = 0;
	}
	tim3_dat++;	
//	IWDG_ReloadCounter();                    //500ms喂狗一次
}


vu8 VCU_Status1_Receive[8];       //VCU_Status1接收报文数组
vu8 VCU_Status2_Receive[8]; 
/* 任务1回调函数（CAN通信接收查询任务） */
void Task1_CallBack(void)
{
	static u8 ERR_Count = 0;             //通信错误计数器
	//printf("Task1\r\n");
	
	if(CAN_Recive_Flag == 1)             //CAN接收成功
	{
		CAN_Recive_Flag = 0;               //清零接收成功标志
		Connection_Flag = 1;               //置位连接成功标志
		ERR_Count = 0;                     //清零通信错误计数器
		if(RTC_Time_flag == 2)
		{
			Calendar.Year = 20*100+VCU_Status2_Receive[1];
			Calendar.Month = VCU_Status2_Receive[2];
			Calendar.Day = VCU_Status2_Receive[3];
			Calendar.Hour = VCU_Status2_Receive[4];
			Calendar.Min = VCU_Status2_Receive[5];
			Calendar.Sec = VCU_Status2_Receive[6];
			RTCvalue = 0x6666;
			RTC_Init();
			RTCvalue = 0x8888;
			RTC_Time_flag++;
		}
		/* 获取车速 */
		if(VCU_Status1_Receive[1] < 99)
		{
			Car_Speed = VCU_Status1_Receive[1];// RxMessage.Data[1]			
		}
    else
		{
			Car_Speed = 35;
		}
		/* 获取油量 */
		if(VCU_Status1_Receive[2] < 99)
		{
			Car_Oil = VCU_Status1_Receive[2];		
		}
	  /* 获取里程 */
		if(Car_Mileage <= (VCU_Status1_Receive[3]+(VCU_Status1_Receive[4]*1.0/100)))
		{
			Car_Mileage = (VCU_Status1_Receive[3]+(VCU_Status1_Receive[4]*1.0/100));
		}
		else
		{
			printf("里程显示数据错误\r\n");
		}
		/* 获取电池电量 */
		if(VCU_Status1_Receive[5] >= 1)
		{
			Batter_flag = VCU_Status1_Receive[5];
		}
		else
		{
			Batter_flag = 1;
		}
		/* 4G信号格数 */
		Network_signal = VCU_Status1_Receive[6];
		if(Batter_flag == 1)
		{
			if(Standby_flag == 1)
			{
			  Standby_mode();
				Standby_flag = 0;
			}
		}
	}
	else
	{
		ERR_Count++;
		if(ERR_Count >= 3)                 //通信错误次数超过3次，任务通信连接异常
		{
			Connection_Flag = 0;             //清零连接成功标志（即通信连接失败）
		}
	}
}


/* 任务2回调函数（RTC更新任务） */
void Task2_CallBack(void)
{
	/* 更新Calendar日历 */
	RTC_Get();
}


/* 任务3回调函数（液晶屏刷新任务） */
void Task3_CallBack(void)
{
  GUI_SetBkColor(GUI_BAK2);
	GUI_SetColor(GUI_WHITE);
	//	/* 刷新日期 */

  GUI_SetFont(GUI_FONT_24_ASCII);
	sprintf((char*)Display_Buf, "%4d/%02d/%02d ", Calendar.Year, Calendar.Month, Calendar.Day);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  GUI_DispStringAt((char*)Display_Buf, 280, 10);
	
	/* 刷新时间 */

	sprintf((char*)Display_Buf, "%02d:%02d:%02d ", Calendar.Hour, Calendar.Min, Calendar.Sec);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  GUI_DispStringAt((char*)Display_Buf, 435, 10);
	
	/* 刷新温度 */
	sprintf((char*)Display_Buf, "%.1f", Temperature/10.0);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	GUI_SetFont(GUI_FONT_20B_ASCII);
  GUI_DispStringAt((char*)Display_Buf, 390, 52);
	
	/* 刷新通信连接状态 */
	if(Connection_Flag == 1)
	{
//		GUI_SetBkColor(GUI_BAK2);
		GUI_SetColor(GUI_YELLOW);
		GUI_DispStringAt("<CAN-OK> ", 165, 428);
	}
	else
	{
		GUI_SetColor(GUI_RED);
		GUI_DispStringAt("<ERROR>", 165, 428);
	}
	/* 刷新速度 */
	_DemoBandingMemdev_Speed(Car_Speed);
	/* 刷新里程 */
	_DemoBandingMemdev_Mileage(Car_Mileage);	
	/* 刷新油量 */
	_DemoBandingMemdev_Oil(Car_Oil);
	/* 刷新电量 */
	_DemoBandingMemdev_Battery(Batter_flag);	
	/* 刷新4G信号 */
	_DemoBandingMemdev_Network(Network_signal);
}

/* 任务4回调函数（仪表温度采集任务） */
void Task4_CallBack(void)
{
//	u8 i = 0;
//	Ds18b20_ReadRom(Ds18b20_ID);
//	for(i = 0;i<8;i++)
//	{
//		printf("%#x  ",Ds18b20_ID[i]);
//	}
//	printf("\r\n");
	Temperature = Ds18b20_ReadSingleTemp(0);
}

void _BkcbWindow(WM_MESSAGE* pMsg)
{
	GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();
		/* 绘制背景图 */
	f_mount(&fsp,"0:",1); //挂载文件系统
	GUI_DrawStreamedBitmapExAuto(_Get_Data, &flp, 0, 0); //绘制流位图
	f_mount(NULL,"0:",0); //解挂文件系统
}
