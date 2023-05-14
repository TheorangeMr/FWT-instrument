/*******************************************
	*文件名 ：  bsp_delay.c
	*作   者：  王峰
	*修改时间： 2021.02.20
	*版   本：  v1.0
  *说   明：  延时函数配置源文件
*******************************************/



#include "bsp_delay.h"


static u8  fac_us=0;							//us延时倍乘因子（1us对应systick时钟数）			   
static u16 fac_ms=0;							//ms延时倍乘因子（1ms对应systick时钟数）
	



/*
	*函数名： Delay_Init()
	*功  能： 延时初始化函数
	*参  数： 无
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.02.20
*/
void Delay_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	 //选择外部时钟  HCLK/8（9MHZ）
	fac_us=SystemCoreClock/8000000;				             //为系统时钟的1/8  
	fac_ms=(u16)fac_us*1000;					             //非OS下,代表每个ms需要的systick时钟数   
}



/*
	*函数名： Delay_Ms()
	*功  能： 延时ms函数
    *参  数： nms:延时ms数
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.02.20
*/
void Delay_Ms(u16 nms)
{
    u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;							//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
	do
	{
		temp=SysTick->CTRL;                     //循环读取SysTick控制寄存器的值
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;       					//清空计数器
}


/*
	*函数名： Delay_Us()
	*功  能： 延时us函数
    *参  数： nus:延时us数
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.02.20
*/
void Delay_Us(u32 nus)
{
    u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					 //时间加载	  		 
	SysTick->VAL=0x00;        					 //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	 //开始倒数	  
	do
	{
		temp=SysTick->CTRL;                      //循环读取SysTick控制寄存器的值
	}while((temp&0x01)&&!(temp&(1<<16)));		 //等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	 //关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器
}








































