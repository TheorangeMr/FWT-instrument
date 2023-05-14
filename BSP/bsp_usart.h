/*******************************************
	*文件名 ：  bsp_usart.h
	*作   者：  王峰
	*修改时间： 2021.04.09
	*版   本：  v1.0
  *说   明：  串口配置头文件
*******************************************/


/*

一、要支持printf函数,需要选择use MicroLIB

二、stm32用printf函数重定向到串口USART1发现第一个字符没打印出来具体如下：
    刚开始修改fputc函数如下：
    int fputc(int ch,FILE *f)
    {
    USART_SendData(USART1,(uint8_t)ch)；
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
    return(ch);
    }

    然后其他外设配置好之后直接使用printf(“abc");发现a打印不出来，具体原因为stm32的usart的sr寄存器的TC为初始为1，
    导致第一次while时的第一个数据被覆盖，所以打印不出；

    具体改进方法:

    1.直接修改while的等待条件，while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);把TC改为TXE，因为TXE
    只能硬件清零，在数据写进DR时就置0，数据移出时置1，可以保证数据不会在DR被覆盖；

    2.既然TC位初始为1导致的问题，那么我们在使用printf函数前把TC置0也可以解决问题，

    写多一句USART1->SR&=~(1<<6)；

    3.条件允许情况下，也可以委屈自己一下，在输出字符前面加一没用字符，也可以解决；



*/

#ifndef __BSP_USART_H
#define __BSP_USART_H
#include "stdio.h"	
#include "stm32f10x.h"


/******************************串口配置相关宏定义***************************/

/* 串口选择（将要使用的串口宏定义置1即可） */
#define DEBUG_USART1     1
#define DEBUG_USART2     0
#define DEBUG_USART3     0
#define DEBUG_USART4     0
#define DEBUG_USART5     0

#if DEBUG_USART1
// 串口1-USART1
#define  DEBUG_USART1_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART1_APBxClkCmd         RCC_APB2PeriphClockCmd

// USART1 GPIO 引脚宏定义
#define  DEBUG_USART1_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART1_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART1_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART1_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART1_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART1_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART1_IRQ                USART1_IRQn
#define  DEBUG_USART1_IRQHandler         USART1_IRQHandler
#endif

#if DEBUG_USART2
//串口2-USART2
#define  DEBUG_USART2_CLK                RCC_APB1Periph_USART2
#define  DEBUG_USART2_APBxClkCmd         RCC_APB1PeriphClockCmd

// USART2 GPIO 引脚宏定义
#define  DEBUG_USART2_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART2_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART2_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART2_TX_GPIO_PIN        GPIO_Pin_2
#define  DEBUG_USART2_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART2_RX_GPIO_PIN        GPIO_Pin_3

#define  DEBUG_USART2_IRQ                USART2_IRQn
#define  DEBUG_USART2_IRQHandler         USART2_IRQHandler
#endif

#if DEBUG_USART3
//串口3-USART3
#define  DEBUG_USART3_CLK                RCC_APB1Periph_USART3
#define  DEBUG_USART3_APBxClkCmd         RCC_APB1PeriphClockCmd

// USART3 GPIO 引脚宏定义
#define  DEBUG_USART3_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  DEBUG_USART3_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART3_TX_GPIO_PORT       GPIOB   
#define  DEBUG_USART3_TX_GPIO_PIN        GPIO_Pin_10
#define  DEBUG_USART3_RX_GPIO_PORT       GPIOB
#define  DEBUG_USART3_RX_GPIO_PIN        GPIO_Pin_11

#define  DEBUG_USART3_IRQ                USART3_IRQn
#define  DEBUG_USART3_IRQHandler         USART3_IRQHandler
#endif

#if DEBUG_USART4
//串口4-UART4
#define  DEBUG_USART4_CLK                RCC_APB1Periph_UART4
#define  DEBUG_USART4_APBxClkCmd         RCC_APB1PeriphClockCmd

// USART4 GPIO 引脚宏定义
#define  DEBUG_USART4_GPIO_CLK           (RCC_APB2Periph_GPIOC)
#define  DEBUG_USART4_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART4_TX_GPIO_PORT       GPIOC   
#define  DEBUG_USART4_TX_GPIO_PIN        GPIO_Pin_10
#define  DEBUG_USART4_RX_GPIO_PORT       GPIOC
#define  DEBUG_USART4_RX_GPIO_PIN        GPIO_Pin_11

#define  DEBUG_USART4_IRQ                UART4_IRQn
#define  DEBUG_USART4_IRQHandler         UART4_IRQHandler
#endif

#if DEBUG_USART5
//串口5-UART5
#define  DEBUG_USART5_CLK                RCC_APB1Periph_UART5
#define  DEBUG_USART5_APBxClkCmd         RCC_APB1PeriphClockCmd

// USART5 GPIO 引脚宏定义
#define  DEBUG_USART5_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
#define  DEBUG_USART5_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART5_TX_GPIO_PORT       GPIOC   
#define  DEBUG_USART5_TX_GPIO_PIN        GPIO_Pin_12
#define  DEBUG_USART5_RX_GPIO_PORT       GPIOD
#define  DEBUG_USART5_RX_GPIO_PIN        GPIO_Pin_2

#define  DEBUG_USART5_IRQ                UART5_IRQn
#define  DEBUG_USART5_IRQHandler         UART5_IRQHandler
#endif


/********************************串口相关函数*******************************/

void Usart1_Init(u32 BaudRate);
void Usart2_Init(u32 BaudRate);
void Usart3_Init(u32 BaudRate);
void Usart4_Init(u32 BaudRate);
void Usart5_Init(u32 BaudRate);
void Usart_SendByte(USART_TypeDef* pUsart_x, u8 zdata);
void Usart_SendHalfWord(USART_TypeDef* pUsart_x, u16 zdata);
void Usart_SendArray(USART_TypeDef* pUsart_x, u8* array, u8 Num);
void Usart_SendString(USART_TypeDef* pUsart_x, u8* str);




#endif /* __BSP_USART_H */



