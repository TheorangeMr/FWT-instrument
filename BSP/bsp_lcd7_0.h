/*******************************************
	*文件名 ：  lcd7_0.h(7寸液晶屏驱动头文件)
	*作   者：  王峰
	*修改时间： 2021.01.05
	*版   本：  v1.0
*******************************************/



#ifndef _LCD7_0_H
#define _LCD7_0_H

#include "sys.h"
#include "bsp_w25qxx.h"


/*   
     液晶屏控制芯片MD070SD 采用 8080 时序 16bit 并行总线接口，8080时序可以由软件模拟，
     也可以由FSMC产生，本驱动采用FSMC产生8080时序进行驱动
*/


/********************************FSMC相关引脚宏定义***********************************/

/* FSMC数据线 */

#define FSMC_D0_GPIO_PORT        GPIOD
#define FSMC_D0_GPIO_CLK         RCC_APB2Periph_GPIOD
#define FSMC_D0_GPIO_PIN         GPIO_Pin_14

#define FSMC_D1_GPIO_PORT        GPIOD
#define FSMC_D1_GPIO_CLK         RCC_APB2Periph_GPIOD
#define FSMC_D1_GPIO_PIN         GPIO_Pin_15

#define FSMC_D2_GPIO_PORT        GPIOD
#define FSMC_D2_GPIO_CLK         RCC_APB2Periph_GPIOD
#define FSMC_D2_GPIO_PIN         GPIO_Pin_0

#define FSMC_D3_GPIO_PORT        GPIOD
#define FSMC_D3_GPIO_CLK         RCC_APB2Periph_GPIOD
#define FSMC_D3_GPIO_PIN         GPIO_Pin_1

#define FSMC_D4_GPIO_PORT        GPIOE
#define FSMC_D4_GPIO_CLK         RCC_APB2Periph_GPIOE
#define FSMC_D4_GPIO_PIN         GPIO_Pin_7

#define FSMC_D5_GPIO_PORT        GPIOE
#define FSMC_D5_GPIO_CLK         RCC_APB2Periph_GPIOE
#define FSMC_D5_GPIO_PIN         GPIO_Pin_8

#define FSMC_D6_GPIO_PORT        GPIOE
#define FSMC_D6_GPIO_CLK         RCC_APB2Periph_GPIOE
#define FSMC_D6_GPIO_PIN         GPIO_Pin_9

#define FSMC_D7_GPIO_PORT        GPIOE
#define FSMC_D7_GPIO_CLK         RCC_APB2Periph_GPIOE
#define FSMC_D7_GPIO_PIN         GPIO_Pin_10

#define FSMC_D8_GPIO_PORT        GPIOE
#define FSMC_D8_GPIO_CLK         RCC_APB2Periph_GPIOE
#define FSMC_D8_GPIO_PIN         GPIO_Pin_11

#define FSMC_D9_GPIO_PORT        GPIOE
#define FSMC_D9_GPIO_CLK         RCC_APB2Periph_GPIOE
#define FSMC_D9_GPIO_PIN         GPIO_Pin_12

#define FSMC_D10_GPIO_PORT       GPIOE
#define FSMC_D10_GPIO_CLK        RCC_APB2Periph_GPIOE
#define FSMC_D10_GPIO_PIN        GPIO_Pin_13

#define FSMC_D11_GPIO_PORT       GPIOE
#define FSMC_D11_GPIO_CLK        RCC_APB2Periph_GPIOE
#define FSMC_D11_GPIO_PIN        GPIO_Pin_14

#define FSMC_D12_GPIO_PORT       GPIOE
#define FSMC_D12_GPIO_CLK        RCC_APB2Periph_GPIOE
#define FSMC_D12_GPIO_PIN        GPIO_Pin_15

#define FSMC_D13_GPIO_PORT       GPIOD
#define FSMC_D13_GPIO_CLK        RCC_APB2Periph_GPIOD
#define FSMC_D13_GPIO_PIN        GPIO_Pin_8

#define FSMC_D14_GPIO_PORT       GPIOD
#define FSMC_D14_GPIO_CLK        RCC_APB2Periph_GPIOD
#define FSMC_D14_GPIO_PIN        GPIO_Pin_9

#define FSMC_D15_GPIO_PORT       GPIOD
#define FSMC_D15_GPIO_CLK        RCC_APB2Periph_GPIOD
#define FSMC_D15_GPIO_PIN        GPIO_Pin_10

/* FSMC控制信号线 */

//片选引脚，连接FSMC_NE1
#define      MD070SD_CS_CLK           RCC_APB2Periph_GPIOD
#define      MD070SD_CS_PORT          GPIOD
#define      MD070SD_CS_PIN           GPIO_Pin_7
//读使能引脚，连接FSMC_NOE
#define      MD070SD_RD_CLK           RCC_APB2Periph_GPIOD
#define      MD070SD_RD_PORT          GPIOD
#define      MD070SD_RD_PIN           GPIO_Pin_4
//写使能引脚，连接FSMC_NWE
#define      MD070SD_WR_CLK           RCC_APB2Periph_GPIOD
#define      MD070SD_WR_PORT          GPIOD
#define      MD070SD_WR_PIN           GPIO_Pin_5
//数据/命令选择引脚，连接FSMC_A16
#define      MD070SD_RS_CLK           RCC_APB2Periph_GPIOD
#define      MD070SD_RS_PORT          GPIOD
#define      MD070SD_RS_PIN           GPIO_Pin_11
//液晶屏复位引脚
#define      MD070SD_RESET_CLK        RCC_APB2Periph_GPIOA
#define      MD070SD_RESET_PORT       GPIOA
#define      MD070SD_RESET_PIN        GPIO_Pin_1


//写数据地址（读数据D/CX也为高）
#define      MD070SD_DATA_ADDR        (vu16 *)(0x60000000 | (1<<(16+1)))
//写命令地址
#define      MD070SD_CMD_ADDR         (vu16 *)(0x60000000 & ~(1<<(16+1)))



/*******************************MD070SD相关寄存器地址*******************************/


#define         BL_REG              0x0001         //地址指向背光亮度寄存器
#define         START_ROW_REG       0x0002         //地址指向起始行地址寄存器
#define         START_COL_REG       0x0003         //地址指向起始列地址寄存器
#define         END_ROW_REG         0x0006         //地址指向结束行地址寄存器
#define         END_COL_REG         0x0007         //地址指向结束列地址寄存器
#define         PAGE_ADDR_REG       0x0004         //显示页寄存器
#define         WR_PAGE_REG         0x0005         //读写页寄存器
#define         SLEEP_REG           0x000B         //休眠状态寄存器
#define         SHOW_STATE_REG      0x000C         //显示状态选择寄存器
#define         ADDR_ADD_REG        0x000D         //地址指向地址增量方向寄存器
#define         DATA_CH_REG         0x000F         //地址指向数据通道


/******************************************液晶屏参数相关宏**************************************/
#define      MD070SD_SCREEN_START_X           0                      //屏幕起始点横坐标
#define      MD070SD_SCREEN_START_Y           0                      //屏幕起始点纵坐标
#define      MD070SD_SCREEN_ROW_WIDTH        480                     //屏幕较窄方向的宽度
#define      MD070SD_SCREEN_COL_WIDTH        800                     //屏幕较宽方向的宽度


/*************************************************RGB565相关宏***********************************************/

//RGB888转RGB565
#define      RGB888_TO_RGB565(r,g,b)         (u16)(((r & 0x1f)<<11) | ((g & 0x3f)<<5) | ((b & 0x1f)))

//常用颜色
#define      WHITE		 		                 0xFFFF	                 //白色
#define      BLACK                         0x0000	                 //黑色 
#define      GREY                          0xF7DE	                 //灰色 
#define      BLUE                          0x001F	                 //蓝色 
#define      BLUE2                         0x051F	                 //浅蓝色 
#define      RED                           0xF800	                 //红色 
#define      MAGENTA                       0xF81F	                 //红紫色，洋红色 
#define      GREEN                         0x07E0	                 //绿色 
#define      CYAN                          0x7FFF	                 //蓝绿色，青色 
#define      YELLOW                        0xFFE0	                 //黄色 
#define      BRED                          0xF81F
#define      GRED                          0xFFE0
#define      GBLUE                         0x07FF

#define      GUI_BAK1                      0x0A0015
#define      GUI_BAK2                      0x000811

/******************************************形状参数结构体****************************************/

//圆
typedef struct {
	u16 center_x,
	    center_y,
	    radius;
}CIRCLE;



/**********************************FSMC和MD070SD相关驱动函数***********************************/


void Fsmc_Config(void);
void MD070SD_Gpio_Config(void);
void MD070SD_Init(void);
void MD070SD_Write_Cmd(u16 cmd);
void MD070SD_Write_Data(u16 zdata);
u16  MD070SD_Read_Data(void);
u16 MD070SD_GetPixel(u16 posx , u16 posy);
void MD070SD_PutPixel(u16 posx , u16 posy , u16 color);
void MD070SD_Open_Window(u16 start_x , u16 start_y , u16 end_x , u16 end_y);
void MD070SD_Draw_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1 , u16 color);
void MD070SD_Draw_Circle(CIRCLE circle , u16 color , u8 mode);
void MD070SD_Draw_Line(u16 start_x , u16 start_y , u16 end_x , u16 end_y , u16 color);
void MD070SD_Draw_Cross(u16 posx , u16 posy);
void MD070SD_ShowChar_En(u16 pos_x , u16 pos_y , u8 zdata, FONT font);
void MD070SD_ShowString_En(u16 pos_x , u16 pos_y , u8 *zdata, FONT font);
void MD070SD_ShowChar_Ch(u16 pos_x , u16 pos_y , u8 *zdata, FONT font);
void MD070SD_ShowString_Ch(u16 pos_x , u16 pos_y , u8 *zdata, FONT font);
void MD070SD_Clear_Screen(u16 start_x , u16 start_y , u16 width , u16 height);
void MD070SD_Set_Txtcolor(u16 color);
void MD070SD_Set_Backcolor(u16 color);
void MD070SD_Set_BackLight(u8 BL_Value);



/* 通过FSMC对MD070SD芯片进行读写，采用内联函数实现 */


//向MD070SD写入命令函数
//2021.01.05
//作者：王峰
//参数：cmd
__inline  void  MD070SD_Write_Cmd(u16 cmd)
{
	* MD070SD_CMD_ADDR = cmd;
}


//向MD070SD写入数据
//2021.01.05
//作者：王峰
//参数：zdata
__inline void MD070SD_Write_Data(u16 zdata)
{
	* MD070SD_DATA_ADDR = zdata;
}


//读取MD070SD数据
//2021.01.05
//作者：王峰
//返回值：读取的数据
__inline u16 MD070SD_Read_Data(void)
{
	return * MD070SD_DATA_ADDR;
}


#endif


