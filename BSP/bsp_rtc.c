#include "bsp_rtc.h"
#include "bsp_delay.h"
#include <stdio.h>

static u8 Is_LeapYear(u16 year);
uint16_t RTCvalue = 0x8888;

////日历结构体
extern CALENDAR Calendar;   //初始化日历结构体

//闹钟结构体
ALARM_TypeDef Alarm = {2023, 5, 24, 2, 21, 43};    //初始化闹钟结构体
//闰年月偏移表
u16 const Leap_month_offset[]={0,31,60,91,121,152,182,213,244,274,305,335};
//平年月偏移表
u16 const month_offset[]={0,31,59,90,120,151,181,212,243,273,304,334};

u16 Init_Year = 1970;  //以1970年1月1日为基准
u8  Init_weekday=4;    //起始星期数为星期四
vu8 AlarmFlag=0;       //闹钟中断标志
vu8 SecFlag=0;         //秒中断标志


/*
	*函数名： RTC_NVIC_Conflig()
	*功  能： RTC中断配置函数
	*参  数： 无
	*返回值： 无
	*作  者： WF
	*日  期： 2021.03.15
*/
void RTC_NVIC_Conflig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
		/* 配置为中断优先级分组1 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		            //RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	    //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	          //响应优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		            //使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		                            //初始化NVIC
}



/*
	*函数名： RTC_Init()
	*功  能： RTC初始化函数
	*参  数： 无
	*返回值： 0表示初始化失败，1表示初始化成功
	*作  者： WF
	*日  期： 2021.03.15
*/
u8 RTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);    //使能电源时钟和备份区域时钟
	PWR_BackupAccessCmd(ENABLE); 	                                           //使能后备寄存器访问
	
  u8 temp = 0;
  
	/* RTC一般只要初始化一次，如果下次要进行配置，只需修改0x8888为其他值即可 */
	if(BKP_ReadBackupRegister(BKP_DR1) != RTCvalue)
	{
		BKP_DeInit();                              //复位备份区域
		RCC_LSEConfig(RCC_LSE_ON);                 // 开启外部低速振荡器
        
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET && temp<250)
		{
			temp++;
			Delay_Ms(10);
		}
		if(temp>=250) return 0;                      //初始化时钟失败,晶振有问题不工作
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);      //选择 LSE 作为 RTC 时钟
    RCC_RTCCLKCmd(ENABLE);                       //使能RTC时钟
		RTC_WaitForLastTask();                       //等待上次操作完成
		RTC_WaitForSynchro();		                     //等待RTC寄存器同步  

		RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR,DISABLE);  //使能秒中断和闹钟中断
		RTC_WaitForLastTask();                        //等待上次操作完成

		/* 修改CNT,ALR,PRL的时候，必须先进入配置模式 */
    RTC_EnterConfigMode();                       //进入配置模式
		RTC_SetPrescaler(32767);                     //设置分频系数32767，时钟频率为1hz,计数周期为1s
		RTC_WaitForLastTask();                       //等待上次操作完成
		RTC_Set(Calendar);                           //设置时间
		RTC_Alarm_Set(Alarm);                        //设置闹钟
		RTC_WaitForLastTask();                       //等待上次操作完成
		RTC_ExitConfigMode();                        //退出配置模式
		BKP_WriteBackupRegister(BKP_DR1,0x8888);     //将0x0524写入BKP_DR1，下次就不会对RTC进行操作
	}
	else
	{
		//每次上电，都要等待等待RTC寄存器同步
		RTC_WaitForSynchro();		                        //等待RTC寄存器同步
		RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR,DISABLE);    //使能秒中断和闹钟中断
		RTC_WaitForLastTask();                          //等待上次操作完成
	}
	RTC_NVIC_Conflig();                             //设置RTC中断
	RTC_Get();                                      //更新时间
	return 1;                                       //初始化成功
}


/*
	*函数名： RTC_Alarm_Set()
	*功  能： 设置RTC闹钟函数
	*参  数： alarm：闹钟结构体
	*返回值： 0表示初始化失败，1表示初始化成功
	*作  者： WF
	*日  期： 2021.03.15
*/
u8 RTC_Alarm_Set(ALARM_TypeDef alarm)
{
	u8 LeapYears=0;
	u8 UnLeapYears=0;
	u16 i;
	u32 alarm_counter=0;
	
	/* 闰年一年31622400秒，平年一年31536000秒 */
	UnLeapYears = alarm.AlarmYear - Init_Year;
	i = alarm.AlarmYear - 1;                       //计算闰平年数时不能包括当前年
	
	/* 计算闰年数 */
	while(i >= Init_Year)
	{
		if(Is_LeapYear(i)) LeapYears++;
		i--;
	}
	UnLeapYears-=LeapYears; //平年 = 总年数 - 闰年数
	alarm_counter+=((LeapYears*31622400)+(UnLeapYears*31536000));
	
	//一天86400秒
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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	                                            //使能RTC和后备寄存器访问 
	RTC_EnterConfigMode();                                                    //进入配置模式
	RTC_SetAlarm(alarm_counter);	                                            //设置RTC的ALR计数器的值，将闹钟写入
	RTC_WaitForLastTask();	                                                  //等待最近一次对RTC寄存器的写操作完成  	
	return 1;
}


/*
	*函数名： RTC_Set()
	*功  能： 设置RTC时间
	*参  数： calendar：日历结构体
	*返回值： 0表示设置失败，1表示设置成功
	*作  者： WF
	*日  期： 2021.03.15
*/
u8 RTC_Set(CALENDAR calendar)
{
	u8 LeapYears=0;
	u8 UnLeapYears=0;
	u16 year = 0;
	u32 Counter=0;
	
	/* 闰年一年31622400秒，平年一年31536000秒 */
	year = calendar.Year - 1;                   //计算闰平年数时不能包括当前年
	while(year >= Init_Year)
	{
		if(Is_LeapYear(year)) LeapYears++;
		year--;
	}
  UnLeapYears = calendar.Year-Init_Year;
	UnLeapYears -= LeapYears;
	Counter += ((LeapYears*31622400)+(UnLeapYears*31536000));
	
	//一天86400秒
	if(Is_LeapYear(calendar.Year))
	{
		Counter += (Leap_month_offset[calendar.Month-1] + calendar.Day-1)*86400;
	}
	else
	{
		Counter += (month_offset[calendar.Month-1] + calendar.Day-1)*86400;
	}
    //减1是因为day是在1月1日的基础上的偏移
	
	Counter += calendar.Hour*3600;
	Counter += calendar.Min*60;
	Counter += calendar.Sec;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	                                            //使能RTC和后备寄存器访问 
	RTC_EnterConfigMode();                                                    //进入配置模式
	RTC_SetCounter(Counter);	                                                //设置RTC计数器的值，将时间日期写入
	RTC_WaitForLastTask();	                                                  //等待最近一次对RTC寄存器的写操作完成  	
	return 1;
}


/*
	*函数名： RTC_Get()
	*功  能： 获取RTC时间函数
	*参  数： 无
	*返回值： 无
	*作  者： WF
	*日  期： 2021.03.15
*/
void RTC_Get(void)
{
	u8 i = 0;
	u32 Counter;
	u32 day=0;
  static u32 DayCnt = 0;                                                    //记录天数
	u16 year = Init_Year;                                                     //以1970年为基准
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	                                            //使能RTC和后备寄存器访问
	Counter=RTC_GetCounter();                                                   
  day = Counter/86400;                                                      //将秒数转化为天数
	
	if(DayCnt != day) //如果记录的天数和读取的天数不一致，则说明日期发生更新
	{
		//获取年份
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
		
		//获取月份与日
		for(i = 0; i < 12; i++)   //天数和月偏移表从小到大依次比较确定月份
		{
			if(Is_LeapYear(year))
			{
			  if(day < Leap_month_offset[i]) break;  //天数小于当前月偏移表的值，则月份就是该月
			}
			else
			{
			  if(day < month_offset[i])      break;  //天数小于当前月偏移表的值，则月份就是该月
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
		//加1是因为day是在1月1日的基础上的偏移
  }
	
	//获取时分秒
	Calendar.Hour = (Counter%86400)/3600;
	Calendar.Min = (Counter%86400%3600)/60;
	Calendar.Sec = Counter%60;
	
	//获取星期数
	Calendar.Weekday = RTC_Get_Week(Calendar.Year, Calendar.Month, Calendar.Day);
}


/*
	*函数名： RTC_Get_Week()
	*功  能： 获取星期数函数
	*参  数： year：年, month：月, day：日
	*返回值： 星期数
	*作  者： WF
	*日  期： 2021.03.15
*/
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{
	u16 DayOffset = 0;    //天偏移数，每次计算后及时取余，防止越界
	u16 i = 0;
	for(i = Init_Year; i<year; i++)
	{
		if(Is_LeapYear(i))
		{
			DayOffset+=2;     //如果是闰年，年偏移天数为2
		}
		else
		{
			DayOffset+=1;     //如果是平年，年偏移天数为1
		}
	}
	DayOffset%=7;         //通过对7取余，转化为星期偏移数
	
	if(Is_LeapYear (year))
	{
		DayOffset = (DayOffset+Leap_month_offset[month-1])%7;    //如果是闰年，加上闰年月偏移天数
	}
	else
	{
		DayOffset = (DayOffset+month_offset[month-1])%7;         //如果是平年，加上平年月偏移天数
	}
	
	DayOffset=(DayOffset+day+Init_weekday-1)%7;                 //加上日偏移天数和星期数基准
	
	return DayOffset;
}



/*
	*函数名： Update_Handle()
	*功  能： RTC更新处理函数
	*参  数： 无
	*返回值： 无
	*作  者： WF
	*日  期： 2021.03.15
*/
void Update_Handle(void)
{
	RTC_Get();
	printf("%d年%d月%d日%d:%d:%d  星期%d \r\n",Calendar.Year, Calendar.Month,
	Calendar.Day, Calendar.Hour, Calendar.Min, Calendar.Sec, Calendar.Weekday);
}


/*
	*函数名： Alarm_Handle()
	*功  能： RTC闹钟处理函数
	*参  数： 无
	*返回值： 无
	*作  者： WF
	*日  期： 2021.03.15
*/
void Alarm_Handle(void)
{
	printf("ALARM! \r\n");
}


/*
	*函数名： Is_LeapYear()
	*功  能： 闰年判断函数
	*参  数： year：年份
	*返回值： 0：表示平年，1：表示闰年
	*作  者： WF
	*日  期： 2021.03.15
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
	*函数名： RTC_IRQHandler()
	*功  能： RTC中断服务函数
	*参  数： 无
	*返回值： 无
	*作  者： WF
	*日  期： 2021.03.15
*/
void RTC_IRQHandler(void)
{
	//秒中断
	if(RTC_GetITStatus(RTC_IT_SEC)==SET)
	{
		SecFlag=1;                                  //秒标志置1
		RTC_ClearITPendingBit(RTC_IT_SEC);          //清除标志
	}
	//闹钟中断
	if(RTC_GetITStatus(RTC_IT_ALR)==SET)
	{
		AlarmFlag=1;                                //闹钟标志置1
		RTC_ClearITPendingBit(RTC_IT_ALR);          //清除标志
	}
}

