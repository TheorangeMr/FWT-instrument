#ifndef _BSP_DS18B20H
#define _BSP_DS18B20H

#include "stm32f10x.h"

/************************************************************************************
																DS18B20引脚宏定义
************************************************************************************/
#define  DS18B20_DQ_GPIO_CLK           (RCC_APB2Periph_GPIOE)
#define  DS18B20_DQ_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DS18B20_DQ_GPIO_PORT          GPIOE   
#define  DS18B20_DQ_GPIO_PIN           GPIO_Pin_4

#define DS18B20_DQ_IN()  {GPIOE->CRL&=0xfff0ffff;GPIOE->CRL|=(u32)8<<16;}//PE4设置为上拉输入
#define DS18B20_DQ_OUT() {GPIOE->CRL&=0xfff0ffff;GPIOE->CRL|=(u32)3<<16;}//PE4设置为推挽输出

#define DS18B20_DQ_SET()   {DS18B20_DQ_GPIO_PORT->BSRR |= DS18B20_DQ_GPIO_PIN;}
#define DS18B20_DQ_RESET() {DS18B20_DQ_GPIO_PORT->BRR |= DS18B20_DQ_GPIO_PIN;}
#define DS18B20_DQ_READ()  ((DS18B20_DQ_GPIO_PORT->IDR & DS18B20_DQ_GPIO_PIN)>>4)


/************************************************************************************
																DS18B20相关命令
************************************************************************************/

#define 	DS18B20_READ_ROM            0x33      // 读ROM指令    
#define 	DS18B20_MATCH_ROM           0x55      // 匹配ROM指令    
#define 	DS18B20_SKIP_ROM            0xCC      // 跳过ROM指令    
#define 	DS18B20_SEARCH_ROM          0xF0      // 搜索ROM指令    
#define 	DS18B20_ALARM_SEARCH     	  0xEC      // 报警搜索指令    
#define 	DS18B20_WRITE_REG     		  0x4E      // 写暂存寄存器指令    
#define 	DS18B20_READ_REG     		    0xBE      // 读暂存寄存器指令    
#define 	DS18B20_COPY_REG   			    0x48      // 复制暂存寄存器指令    
#define 	DS18B20_CONVERT         	  0x44      // 启动温度转换指令    
#define 	DS18B20_RECALL         		  0xB8      // 重新调出E2PROM的数据  


/***********************************************************************************
												      DS18B20多点宏定义
***********************************************************************************/
#define DS18B20_NUM 2   //DS18B20温度传感器数量

#define DS18B20_Module1 0
#define DS18B20_Module2 1



/************************************************************************************
													  DS18B20函数接口声明
************************************************************************************/
void Ds18b20_GPIO_Config(void);
u8 Ds18b20_Reset(void);
u8 Ds18b20_Init(void);
void Ds18b20_WriteByte(u8 zdata);
u8 Ds18b20_ReadByte(void);
u16 Ds18b20_ReadSingleTemp(u8 Num);
void Ds18b20_ReadMultiTemp(u16* TempBuffer);
void Ds18b20_ReadRom(u8 *zdata);
void Ds18b20_MatchRom(u8 *zdata);




#endif /* !_BSP_DS18B20H */

