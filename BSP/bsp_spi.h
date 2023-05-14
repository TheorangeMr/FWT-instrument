/*******************************************
	*文件名 ：  bsp_spi.h
	*作   者：  王峰
	*修改时间： 2021.02.20
	*版   本：  v1.0
    *说   明：  硬件SPI配置头文件
*******************************************/

/*
    SPI的片选引脚配置放到具体的硬件驱动中实现
*/



#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f10x.h"


/************************************** SPI配置相关宏定义 *****************************************/
#define        USER_SPI_X                           SPI2                              //SPIX选择
#define        USER_SPI_APBX_CLOCK_FUN              RCC_APB1PeriphClockCmd            //SPI时钟使能函数
#define        USER_SPI_APBX_CLOCK                  (RCC_APB1Periph_SPI2)             //SPI时钟
#define        USER_SPI_GPIO_APBxClock_FUN          RCC_APB2PeriphClockCmd            //SPI引脚使能函数
#define        USER_SPI_GPIO_APBxClock              (RCC_APB2Periph_GPIOB)            //GPIO时钟

#define        USER_SPI_CLK_PORT                    GPIOB                             //SPI时钟
#define        USER_SPI_CLK_PIN                     GPIO_Pin_13

#define        USER_SPI_MISO_PORT                   GPIOB                             //SPI的MISO
#define        USER_SPI_MISO_PIN                    GPIO_Pin_14

#define        USER_SPI_MOSI_PORT                   GPIOB                             //SPI的MOSI
#define        USER_SPI_MOSI_PIN                    GPIO_Pin_15


/*****************************************SPI相关功能函数******************************************/

void MySPI_Init(void);
u8 SPI_ReadWriteByte(u8 zdata);
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler);


#endif
	

