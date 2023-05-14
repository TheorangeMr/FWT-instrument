#include "bsp_rtc.h"
#include "bsp_delay.h"
#include <stdio.h>

static u8 Is_LeapYear(u16 year);
uint16_t RTCvalue = 0x8888;

////�����ṹ��
extern CALENDAR Calendar;   //��ʼ�������ṹ��

//���ӽṹ��
ALARM_TypeDef Alarm = {2023, 5, 24, 2, 21, 43};    //��ʼ�����ӽṹ��
//������ƫ�Ʊ�
u16 const Leap_month_offset[]={0,31,60,91,121,152,182,213,244,274,305,335};
//ƽ����ƫ�Ʊ�
u16 const month_offset[]={0,31,59,90,120,151,181,212,243,273,304,334};

u16 Init_Year = 1970;  //��1970��1��1��Ϊ��׼
u8  Init_weekday=4;    //��ʼ������Ϊ������
vu8 AlarmFlag=0;       //�����жϱ�־
vu8 SecFlag=0;         //���жϱ�־


/*
	*�������� RTC_NVIC_Conflig()
	*��  �ܣ� RTC�ж����ú���
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.15
*/
void RTC_NVIC_Conflig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
		/* ����Ϊ�ж����ȼ�����1 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		            //RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	    //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	          //��Ӧ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		            //ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);		                            //��ʼ��NVIC
}



/*
	*�������� RTC_Init()
	*��  �ܣ� RTC��ʼ������
	*��  ���� ��
	*����ֵ�� 0��ʾ��ʼ��ʧ�ܣ�1��ʾ��ʼ���ɹ�
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.15
*/
u8 RTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);    //ʹ�ܵ�Դʱ�Ӻͱ�������ʱ��
	PWR_BackupAccessCmd(ENABLE); 	                                           //ʹ�ܺ󱸼Ĵ�������
	
  u8 temp = 0;
  
	/* RTCһ��ֻҪ��ʼ��һ�Σ�����´�Ҫ�������ã�ֻ���޸�0x8888Ϊ����ֵ���� */
	if(BKP_ReadBackupRegister(BKP_DR1) != RTCvalue)
	{
		BKP_DeInit();                              //��λ��������
		RCC_LSEConfig(RCC_LSE_ON);                 // �����ⲿ��������
        
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET && temp<250)
		{
			temp++;
			Delay_Ms(10);
		}
		if(temp>=250) return 0;                      //��ʼ��ʱ��ʧ��,���������ⲻ����
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);      //ѡ�� LSE ��Ϊ RTC ʱ��
    RCC_RTCCLKCmd(ENABLE);                       //ʹ��RTCʱ��
		RTC_WaitForLastTask();                       //�ȴ��ϴβ������
		RTC_WaitForSynchro();		                     //�ȴ�RTC�Ĵ���ͬ��  

		RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR,DISABLE);  //ʹ�����жϺ������ж�
		RTC_WaitForLastTask();                        //�ȴ��ϴβ������

		/* �޸�CNT,ALR,PRL��ʱ�򣬱����Ƚ�������ģʽ */
    RTC_EnterConfigMode();                       //��������ģʽ
		RTC_SetPrescaler(32767);                     //���÷�Ƶϵ��32767��ʱ��Ƶ��Ϊ1hz,��������Ϊ1s
		RTC_WaitForLastTask();                       //�ȴ��ϴβ������
		RTC_Set(Calendar);                           //����ʱ��
		RTC_Alarm_Set(Alarm);                        //��������
		RTC_WaitForLastTask();                       //�ȴ��ϴβ������
		RTC_ExitConfigMode();                        //�˳�����ģʽ
		BKP_WriteBackupRegister(BKP_DR1,0x8888);     //��0x0524д��BKP_DR1���´ξͲ����RTC���в���
	}
	else
	{
		//ÿ���ϵ磬��Ҫ�ȴ��ȴ�RTC�Ĵ���ͬ��
		RTC_WaitForSynchro();		                        //�ȴ�RTC�Ĵ���ͬ��
		RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR,DISABLE);    //ʹ�����жϺ������ж�
		RTC_WaitForLastTask();                          //�ȴ��ϴβ������
	}
	RTC_NVIC_Conflig();                             //����RTC�ж�
	RTC_Get();                                      //����ʱ��
	return 1;                                       //��ʼ���ɹ�
}


/*
	*�������� RTC_Alarm_Set()
	*��  �ܣ� ����RTC���Ӻ���
	*��  ���� alarm�����ӽṹ��
	*����ֵ�� 0��ʾ��ʼ��ʧ�ܣ�1��ʾ��ʼ���ɹ�
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.15
*/
u8 RTC_Alarm_Set(ALARM_TypeDef alarm)
{
	u8 LeapYears=0;
	u8 UnLeapYears=0;
	u16 i;
	u32 alarm_counter=0;
	
	/* ����һ��31622400�룬ƽ��һ��31536000�� */
	UnLeapYears = alarm.AlarmYear - Init_Year;
	i = alarm.AlarmYear - 1;                       //������ƽ����ʱ���ܰ�����ǰ��
	
	/* ���������� */
	while(i >= Init_Year)
	{
		if(Is_LeapYear(i)) LeapYears++;
		i--;
	}
	UnLeapYears-=LeapYears; //ƽ�� = ������ - ������
	alarm_counter+=((LeapYears*31622400)+(UnLeapYears*31536000));
	
	//һ��86400��
	if(Is_LeapYear(alarm.AlarmYear))
	{
		alarm_counter+=(Leap_month_offset[alarm.AlarmMonth - 1]+alarm.AlarmDay - 1)*86400;
	}
	else
	{
		alarm_counter+=(month_offset[alarm.AlarmMonth - 1]+alarm.AlarmDay - 1)*86400;
	}
	
	alarm_counter += alarm.AlarmHour*3600;
	alarm_counter += alarm.AlarmMin*60;
	alarm_counter += alarm.AlarmSec;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	                                            //ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_EnterConfigMode();                                                    //��������ģʽ
	RTC_SetAlarm(alarm_counter);	                                            //����RTC��ALR��������ֵ��������д��
	RTC_WaitForLastTask();	                                                  //�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	return 1;
}


/*
	*�������� RTC_Set()
	*��  �ܣ� ����RTCʱ��
	*��  ���� calendar�������ṹ��
	*����ֵ�� 0��ʾ����ʧ�ܣ�1��ʾ���óɹ�
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.15
*/
u8 RTC_Set(CALENDAR calendar)
{
	u8 LeapYears=0;
	u8 UnLeapYears=0;
	u16 year = 0;
	u32 Counter=0;
	
	/* ����һ��31622400�룬ƽ��һ��31536000�� */
	year = calendar.Year - 1;                   //������ƽ����ʱ���ܰ�����ǰ��
	while(year >= Init_Year)
	{
		if(Is_LeapYear(year)) LeapYears++;
		year--;
	}
  UnLeapYears = calendar.Year-Init_Year;
	UnLeapYears -= LeapYears;
	Counter += ((LeapYears*31622400)+(UnLeapYears*31536000));
	
	//һ��86400��
	if(Is_LeapYear(calendar.Year))
	{
		Counter += (Leap_month_offset[calendar.Month-1] + calendar.Day-1)*86400;
	}
	else
	{
		Counter += (month_offset[calendar.Month-1] + calendar.Day-1)*86400;
	}
    //��1����Ϊday����1��1�յĻ����ϵ�ƫ��
	
	Counter += calendar.Hour*3600;
	Counter += calendar.Min*60;
	Counter += calendar.Sec;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	                                            //ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_EnterConfigMode();                                                    //��������ģʽ
	RTC_SetCounter(Counter);	                                                //����RTC��������ֵ����ʱ������д��
	RTC_WaitForLastTask();	                                                  //�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	return 1;
}


/*
	*�������� RTC_Get()
	*��  �ܣ� ��ȡRTCʱ�亯��
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.15
*/
void RTC_Get(void)
{
	u8 i = 0;
	u32 Counter;
	u32 day=0;
  static u32 DayCnt = 0;                                                    //��¼����
	u16 year = Init_Year;                                                     //��1970��Ϊ��׼
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	                                            //ʹ��RTC�ͺ󱸼Ĵ�������
	Counter=RTC_GetCounter();                                                   
  day = Counter/86400;                                                      //������ת��Ϊ����
	
	if(DayCnt != day) //�����¼�������Ͷ�ȡ��������һ�£���˵�����ڷ�������
	{
		//��ȡ���
		while(day>=365)
		{
			if(Is_LeapYear(year))
			{
				if(day>=366) day -= 366;
				else break;
			}
			else day -= 365;
			year++;
		}
		Calendar.Year = year;
		
		//��ȡ�·�����
		for(i = 0; i < 12; i++)   //��������ƫ�Ʊ��С�������αȽ�ȷ���·�
		{
			if(Is_LeapYear(year))
			{
			  if(day < Leap_month_offset[i]) break;  //����С�ڵ�ǰ��ƫ�Ʊ��ֵ�����·ݾ��Ǹ���
			}
			else
			{
			  if(day < month_offset[i])      break;  //����С�ڵ�ǰ��ƫ�Ʊ��ֵ�����·ݾ��Ǹ���
			}
		}
		Calendar.Month = i;
	
		if(Is_LeapYear(year))
		{
			Calendar.Day = (day-Leap_month_offset[i-1] + 1);
		}
		else 
		{			
			Calendar.Day = (day-month_offset[i-1] + 1);
		}
		//��1����Ϊday����1��1�յĻ����ϵ�ƫ��
  }
	
	//��ȡʱ����
	Calendar.Hour = (Counter%86400)/3600;
	Calendar.Min = (Counter%86400%3600)/60;
	Calendar.Sec = Counter%60;
	
	//��ȡ������
	Calendar.Weekday = RTC_Get_Week(Calendar.Year, Calendar.Month, Calendar.Day);
}


/*
	*�������� RTC_Get_Week()
	*��  �ܣ� ��ȡ����������
	*��  ���� year����, month����, day����
	*����ֵ�� ������
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.15
*/
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{
	u16 DayOffset = 0;    //��ƫ������ÿ�μ����ʱȡ�࣬��ֹԽ��
	u16 i = 0;
	for(i = Init_Year; i<year; i++)
	{
		if(Is_LeapYear(i))
		{
			DayOffset+=2;     //��������꣬��ƫ������Ϊ2
		}
		else
		{
			DayOffset+=1;     //�����ƽ�꣬��ƫ������Ϊ1
		}
	}
	DayOffset%=7;         //ͨ����7ȡ�࣬ת��Ϊ����ƫ����
	
	if(Is_LeapYear (year))
	{
		DayOffset = (DayOffset+Leap_month_offset[month-1])%7;    //��������꣬����������ƫ������
	}
	else
	{
		DayOffset = (DayOffset+month_offset[month-1])%7;         //�����ƽ�꣬����ƽ����ƫ������
	}
	
	DayOffset=(DayOffset+day+Init_weekday-1)%7;                 //������ƫ����������������׼
	
	return DayOffset;
}



/*
	*�������� Update_Handle()
	*��  �ܣ� RTC���´�����
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.15
*/
void Update_Handle(void)
{
	RTC_Get();
	printf("%d��%d��%d��%d:%d:%d  ����%d \r\n",Calendar.Year, Calendar.Month,
	Calendar.Day, Calendar.Hour, Calendar.Min, Calendar.Sec, Calendar.Weekday);
}


/*
	*�������� Alarm_Handle()
	*��  �ܣ� RTC���Ӵ�����
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.15
*/
void Alarm_Handle(void)
{
	printf("ALARM! \r\n");
}


/*
	*�������� Is_LeapYear()
	*��  �ܣ� �����жϺ���
	*��  ���� year�����
	*����ֵ�� 0����ʾƽ�꣬1����ʾ����
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.15
*/
static u8 Is_LeapYear(u16 year)
{
	if((year%4==0&&year%100!=0) || (year%400==0))
	{
		return 1;
	}
	else         
	{
		return 0;
	}      
}


/*
	*�������� RTC_IRQHandler()
	*��  �ܣ� RTC�жϷ�����
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� WF
	*��  �ڣ� 2021.03.15
*/
void RTC_IRQHandler(void)
{
	//���ж�
	if(RTC_GetITStatus(RTC_IT_SEC)==SET)
	{
		SecFlag=1;                                  //���־��1
		RTC_ClearITPendingBit(RTC_IT_SEC);          //�����־
	}
	//�����ж�
	if(RTC_GetITStatus(RTC_IT_ALR)==SET)
	{
		AlarmFlag=1;                                //���ӱ�־��1
		RTC_ClearITPendingBit(RTC_IT_ALR);          //�����־
	}
}

