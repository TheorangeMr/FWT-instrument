/*******************************************
	*文件名 ：  bsp_w25qxx.h
	*作   者：  王峰
	*修改时间： 2021.02.20
	*版   本：  v1.0
  *说   明：  W25QXX芯片驱动头文件
*******************************************/

#ifndef _BSP_W25QXX_H
#define _BSP_W25QXX_H
#include "sys.h"

/****************************字库地址****************************/

//GB2312_16X16字库存储首地址 
#define GB2312_16x16_START_ADDR          (u32)0x000000
//GB2312_24X24字库存储首地址
#define GB2312_24x24_START_ADDR          (u32)(0x000000 + 65*4096)
//GB2312_32X32字库存储首地址
#define GB2312_32x32_START_ADDR          (u32)(0x000000 + 210*4096)
//ASCII_8X16字库存储首地址
#define ASCII_8x16_START_ADDR            (u32)(0x000000 + 466*4096)
//ASCII_12X24字库存储首地址
#define ASCII_12x24_START_ADDR           (u32)(0x000000 + 468*4096)
//ASCII_8X16字库存储首地址
#define ASCII_16x32_START_ADDR           (u32)(0x000000 + 474*4096)
//自定义数字NewDigtal字库首地址
#define NewDigtal_START_ADDR             (u32)(0x000000 + 476*4096)


/**************************Flash字库字体枚举***********************/

typedef enum
{
	GB2312_16x16 = 0,
	GB2312_24x24,
	GB2312_32x32,
	ASCII_8X16,
	ASCII_12X24,
	ASCII_16X32,
}FONT;



/********************************W25X系列/Q系列芯片列表********************************/	   
//W25Q08  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16	
//W25Q128 ID  0XEF17	
#define W25Q08 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

/****************************** W25Q系列芯片片选引脚宏定义 *****************************/
#define W25QXX_CS_GPIO_APBxClock_FUN          RCC_APB2PeriphClockCmd            //片选引脚时钟使能函数
#define W25QXX_CS_GPIO_APBxClock              (RCC_APB2Periph_GPIOB)            //片选引脚GPIO时钟
#define W25QXX_CS_PORT                        GPIOB                             //片选引脚端口
#define W25QXX_CS_PIN                         GPIO_Pin_12                       //片选引脚号

#define Flash_CS_L                            GPIO_ResetBits(GPIOB,GPIO_Pin_12) //Flash片选使能
#define Flash_CS_H                            GPIO_SetBits(GPIOB,GPIO_Pin_12)   //Flash片选禁能



/*********************************W25Q系列芯片指令表**********************************/
#define W25X_WriteEnable		                   0x06       //写使能
#define W25X_WriteDisable		                   0x04       //写禁能
#define W25X_ReadStatusReg		                 0x05       //读状态寄存器
#define W25X_WriteStatusReg		                 0x01       //写状态寄存器
#define W25X_ReadData			                     0x03       //读数据
#define W25X_FastReadData		                   0x0B       //快读
#define W25X_FastReadDual		                   0x3B       //快读双输出
#define W25X_PageProgram		                   0x02       //页编程
#define W25X_BlockErase			                   0xD8       //块檫除
#define W25X_SectorErase		                   0x20       //扇区檫除
#define W25X_ChipErase			                   0xC7       //全芯片檫除
#define W25X_PowerDown			                   0xB9       //掉电
#define W25X_ReleasePowerDown	                 0xAB       //释放掉电
#define W25X_DeviceID			                     0xAB       //读出器件ID
#define W25X_ManufactDeviceID	                 0x90       
#define W25X_JedecDeviceID		                 0x9F       

/*************************************W25Q系列芯片功能函数*********************************************/
void W25QXX_Init(void);                                                     //W25QXX初始化
u16  W25QXX_ReadID(void);  	    		                                        //读取FLASH ID
u8	 W25QXX_ReadSR(void);        		                                        //读取状态寄存器 
void W25QXX_Write_SR(u8 SRdata);  			                                    //写状态寄存器
void W25QXX_Write_Enable(void);  		                                        //写使能 
void W25QXX_Write_Disable(void);		                                        //写保护
void W25Qxx_Sector_Copy(u8 *DataBuffer,u32 WriteAddr,u16 Bytenum);          //复制数据
void W25QXX_Read(u8* DataBuffer,u32 ReadAddr,u16 Bytenum);                  //读取flash
void W25QXX_Fast_Read(u8* DataBuffer,u32 ReadAddr,u16 Bytenum);             //快速读取FLASH
void W25QXX_Write(u8* DataBuffer,u32 WriteAddr,u16 Bytenum);                //写入数据
void W25QXX_Write_Sector(u8 *DataBuffer,u32 Sector_Addr);                   //写入一个扇区的数据
void W25QXX_Write_Page(u8* DataBuffer,u32 WriteAddr,u16 Bytenum);           //写入一页数据
void W25QXX_Erase_Chip(void);    	  	                                      //整片擦除
void W25QXX_Erase_Block(u32 Block_Addr);	                                  //块区擦除
void W25QXX_Erase_Sector(u32 Sector_Addr);	                                //扇区擦除
void W25QXX_Wait_Busy(void);           	                                    //等待空闲
void W25QXX_PowerDown(void);        	                                      //进入掉电模式
void W25QXX_WAKEUP(void);	                                                  //唤醒

/******************************机内码转转字库地址函数****************************/

u32  GB2312_Addr_Tans(u8* code, FONT font);
u32  ASCII_Addr_Tans(char code, FONT font);

#endif

