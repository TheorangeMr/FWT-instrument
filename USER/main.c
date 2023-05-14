/*******************************************
	*�ļ��� ��  main.c
	*��   �ߣ�  WF
	*�޸�ʱ�䣺 2022.05.10
	*��   ����  v2.0
  *˵   ����  ������Դ�ļ�
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
															�ⲿ��������
*******************************************************************************/
extern CALENDAR Calendar;               //RTC����
extern TaskManage_t stTaskManager;      //���������
WM_HWIN hWnd;
/******************************************************************************
															�ⲿ��������
*******************************************************************************/
extern void _DemoBandingMemdev_Speed(u8 zdata);
extern void _DemoBandingMemdev_Oil( u8 zdata);
extern void _DemoBandingMemdev_Mileage( float zdata);
extern void _DemoBandingMemdev_Battery(float zdata);
extern void _DemoBandingMemdev_Network(float zdata);
void _BkcbWindow(WM_MESSAGE* pMsg);

/******************************************************************************
															���غ���������
*******************************************************************************/
FATFS fsp;                        //�ļ�ϵͳ���
BYTE work[FF_MAX_SS] = {0};       //����������
FIL flp;                          //�ļ����
FATFS *pfs;                       //ָ���ļ�ϵͳ��ָ��
FATFS *pfs;                       //ָ���ļ�ϵͳ��ָ��
uint8_t Display_Buf[20] = {0};    //Һ������ʾ��������
vu8 CAN_Recive_Flag = 0;          //CAN���ձ�־
vu8 VCU_Status1_Receive[8];       //VCU_Status1���ձ�������
vu8 VCU_Status2_Receive[8];       //VCU_Status2���ձ�������
u8 Connection_Flag = 0;           //ͨ�����ӱ�־
u8 Car_Speed = 0;                 //����
u8 Car_Oil = 0;                   //����
float Car_Mileage = 0;            //���
u16 Temperature = 0;              //�Ǳ��¶�
uint8_t Batter_flag = 4;          //��ص�����
uint8_t Network_signal = 1;       //4G�ź�
uint8_t RTC_Time_flag = 0;
CALENDAR Calendar;


uint8_t Standby_flag = 0;

extern uint16_t RTCvalue;

u8 wendudat[10] = {5};
//u8 Ds18b20_ID[10] = {0};

/******************************************************************************
															���غ�������
*******************************************************************************/
void Bsp_Init(void);
void Task1_CallBack(void);
void Task2_CallBack(void);
void Task3_CallBack(void);
void Task4_CallBack(void);
int _Get_Data(void * p,const U8 ** ppData, unsigned NumBytesReq, U32 offset);


int main()
{
	/* �弶�����ʼ�� */
	Bsp_Init();
	/* STemWin��ʼ�� */
	GUI_Init();
	
	hWnd = WM_CreateWindow(0,0,800,480,WM_CF_BGND,_BkcbWindow,0);
	
	Temperature = Ds18b20_ReadSingleTemp(0);          //��ȡ�¶�  
	CAN_Config();                                     //����CANģ��
	
//	Iwdg_Init(4, 625);                                //�������Ź���ʼ�����������ʱ��1s
	
	Task_Manager_Init();                              //�����������ʼ��
	Task_Regsiter(0, 20, Task1_CallBack);             //ע������1
	Task_Regsiter(1, 20, Task2_CallBack);             //ע������2
	Task_Regsiter(2, 20, Task3_CallBack);             //ע������3
	Task_Regsiter(3, 100, Task4_CallBack);            //ע������4
	
	Timer2_Init(720-1, 1000-1);                       //��ʱ��2��ʼ���������������
	while(1)
	{
		Task_RunCheck(); //�������������˶�
		Task_Process();  //�������д���
	}
}



 /* ��λͼ���ݻ�ȡ���� */
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



/* �弶�����ʼ������ */
void Bsp_Init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE); //ʹ��CRCʱ��
	Delay_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Usart1_Init(115200);
	RTC_Init();
  //W25QXX_Init();
  MD070SD_Init();
  Delay_Ms(500);
	wendudat[8] = Ds18b20_Init();
	Timer3_Init(7200-1, 5000-1);                      //��ʱ��3��ʼ��      500ms
}

/* ��ʱ��2�����жϷ�����������ʱ�ӽ��ļ����� */
void TIM2_IRQHandler(void)
{
	TIM2->SR = (uint16_t)~TIM_IT_Update;              //����жϱ�־
	
	u8 i = 0;
	
	for(i = 0; i < TASK_NUM_MAX; i++)
	{
		if(stTaskManager.Task[i].Fuc != NULL)
		{
			stTaskManager.Task[i].Count_Time++;
		}
	}
}

/* ��ʱ��3�����жϷ����������Ź�ι���� */
void TIM3_IRQHandler(void)
{
	static uint32_t tim3_dat = 0;
	TIM3->SR = (uint16_t)~TIM_IT_Update;     //����жϱ�־
	if(tim3_dat >= 6000)
	{
		Standby_flag = 1;
		tim3_dat = 0;
	}
	tim3_dat++;	
//	IWDG_ReloadCounter();                    //500msι��һ��
}


vu8 VCU_Status1_Receive[8];       //VCU_Status1���ձ�������
vu8 VCU_Status2_Receive[8]; 
/* ����1�ص�������CANͨ�Ž��ղ�ѯ���� */
void Task1_CallBack(void)
{
	static u8 ERR_Count = 0;             //ͨ�Ŵ��������
	//printf("Task1\r\n");
	
	if(CAN_Recive_Flag == 1)             //CAN���ճɹ�
	{
		CAN_Recive_Flag = 0;               //������ճɹ���־
		Connection_Flag = 1;               //��λ���ӳɹ���־
		ERR_Count = 0;                     //����ͨ�Ŵ��������
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
		/* ��ȡ���� */
		if(VCU_Status1_Receive[1] < 99)
		{
			Car_Speed = VCU_Status1_Receive[1];// RxMessage.Data[1]			
		}
    else
		{
			Car_Speed = 35;
		}
		/* ��ȡ���� */
		if(VCU_Status1_Receive[2] < 99)
		{
			Car_Oil = VCU_Status1_Receive[2];		
		}
	  /* ��ȡ��� */
		if(Car_Mileage <= (VCU_Status1_Receive[3]+(VCU_Status1_Receive[4]*1.0/100)))
		{
			Car_Mileage = (VCU_Status1_Receive[3]+(VCU_Status1_Receive[4]*1.0/100));
		}
		else
		{
			printf("�����ʾ���ݴ���\r\n");
		}
		/* ��ȡ��ص��� */
		if(VCU_Status1_Receive[5] >= 1)
		{
			Batter_flag = VCU_Status1_Receive[5];
		}
		else
		{
			Batter_flag = 1;
		}
		/* 4G�źŸ��� */
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
		if(ERR_Count >= 3)                 //ͨ�Ŵ����������3�Σ�����ͨ�������쳣
		{
			Connection_Flag = 0;             //�������ӳɹ���־����ͨ������ʧ�ܣ�
		}
	}
}


/* ����2�ص�������RTC�������� */
void Task2_CallBack(void)
{
	/* ����Calendar���� */
	RTC_Get();
}


/* ����3�ص�������Һ����ˢ������ */
void Task3_CallBack(void)
{
  GUI_SetBkColor(GUI_BAK2);
	GUI_SetColor(GUI_WHITE);
	//	/* ˢ������ */

  GUI_SetFont(GUI_FONT_24_ASCII);
	sprintf((char*)Display_Buf, "%4d/%02d/%02d ", Calendar.Year, Calendar.Month, Calendar.Day);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  GUI_DispStringAt((char*)Display_Buf, 280, 10);
	
	/* ˢ��ʱ�� */

	sprintf((char*)Display_Buf, "%02d:%02d:%02d ", Calendar.Hour, Calendar.Min, Calendar.Sec);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  GUI_DispStringAt((char*)Display_Buf, 435, 10);
	
	/* ˢ���¶� */
	sprintf((char*)Display_Buf, "%.1f", Temperature/10.0);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	GUI_SetFont(GUI_FONT_20B_ASCII);
  GUI_DispStringAt((char*)Display_Buf, 390, 52);
	
	/* ˢ��ͨ������״̬ */
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
	/* ˢ���ٶ� */
	_DemoBandingMemdev_Speed(Car_Speed);
	/* ˢ����� */
	_DemoBandingMemdev_Mileage(Car_Mileage);	
	/* ˢ������ */
	_DemoBandingMemdev_Oil(Car_Oil);
	/* ˢ�µ��� */
	_DemoBandingMemdev_Battery(Batter_flag);	
	/* ˢ��4G�ź� */
	_DemoBandingMemdev_Network(Network_signal);
}

/* ����4�ص��������Ǳ��¶Ȳɼ����� */
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
		/* ���Ʊ���ͼ */
	f_mount(&fsp,"0:",1); //�����ļ�ϵͳ
	GUI_DrawStreamedBitmapExAuto(_Get_Data, &flp, 0, 0); //������λͼ
	f_mount(NULL,"0:",0); //����ļ�ϵͳ
}
