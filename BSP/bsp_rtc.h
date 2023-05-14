#ifndef BSP_RTC_H
#define BSP_RTC_H


#include "stm32f10x.h"

//日历结构体(公历日月年星期)
typedef struct 
{			
  vu16 Year;
	vu8  Month;
	vu8  Day;
	vu8  Weekday;	
  vu8  Hour;
	vu8  Min;
	vu8  Sec;
}CALENDAR;	


//闹钟参数结构体
typedef struct
{
    u16 AlarmYear;
    u8 AlarmMonth;
    u8 AlarmDay;
    u8 AlarmHour;
    u8 AlarmMin;
    u8 AlarmSec;
    
}ALARM_TypeDef;


/******************************RTC功能函数*******************************/
void RTC_NVIC_Conflig(void);
u8 RTC_Init(void);
u8 RTC_Alarm_Set(ALARM_TypeDef alarm);
void RTC_Get(void); 
u8 RTC_Set(CALENDAR calendar);
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
void Update_Handle(void);
void Alarm_Handle(void);







#endif /* !BSP_RTC_H */

