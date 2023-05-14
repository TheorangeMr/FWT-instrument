/*******************************************
	*�ļ��� ��  bsp_usart.h
	*��   �ߣ�  ����
	*�޸�ʱ�䣺 2021.04.09
	*��   ����  v1.0
  *˵   ����  ��������ͷ�ļ�
*******************************************/


/*

һ��Ҫ֧��printf����,��Ҫѡ��use MicroLIB

����stm32��printf�����ض��򵽴���USART1���ֵ�һ���ַ�û��ӡ�����������£�
    �տ�ʼ�޸�fputc�������£�
    int fputc(int ch,FILE *f)
    {
    USART_SendData(USART1,(uint8_t)ch)��
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
    return(ch);
    }

    Ȼ�������������ú�֮��ֱ��ʹ��printf(��abc");����a��ӡ������������ԭ��Ϊstm32��usart��sr�Ĵ�����TCΪ��ʼΪ1��
    ���µ�һ��whileʱ�ĵ�һ�����ݱ����ǣ����Դ�ӡ������

    ����Ľ�����:

    1.ֱ���޸�while�ĵȴ�������while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);��TC��ΪTXE����ΪTXE
    ֻ��Ӳ�����㣬������д��DRʱ����0�������Ƴ�ʱ��1�����Ա�֤���ݲ�����DR�����ǣ�

    2.��ȻTCλ��ʼΪ1���µ����⣬��ô������ʹ��printf����ǰ��TC��0Ҳ���Խ�����⣬

    д��һ��USART1->SR&=~(1<<6)��

    3.������������£�Ҳ����ί���Լ�һ�£�������ַ�ǰ���һû���ַ���Ҳ���Խ����



*/

#ifndef __BSP_USART_H
#define __BSP_USART_H
#include "stdio.h"	
#include "stm32f10x.h"


/******************************����������غ궨��***************************/

/* ����ѡ�񣨽�Ҫʹ�õĴ��ں궨����1���ɣ� */
#define DEBUG_USART1     1
#define DEBUG_USART2     0
#define DEBUG_USART3     0
#define DEBUG_USART4     0
#define DEBUG_USART5     0

#if DEBUG_USART1
// ����1-USART1
#define  DEBUG_USART1_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART1_APBxClkCmd         RCC_APB2PeriphClockCmd

// USART1 GPIO ���ź궨��
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
//����2-USART2
#define  DEBUG_USART2_CLK                RCC_APB1Periph_USART2
#define  DEBUG_USART2_APBxClkCmd         RCC_APB1PeriphClockCmd

// USART2 GPIO ���ź궨��
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
//����3-USART3
#define  DEBUG_USART3_CLK                RCC_APB1Periph_USART3
#define  DEBUG_USART3_APBxClkCmd         RCC_APB1PeriphClockCmd

// USART3 GPIO ���ź궨��
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
//����4-UART4
#define  DEBUG_USART4_CLK                RCC_APB1Periph_UART4
#define  DEBUG_USART4_APBxClkCmd         RCC_APB1PeriphClockCmd

// USART4 GPIO ���ź궨��
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
//����5-UART5
#define  DEBUG_USART5_CLK                RCC_APB1Periph_UART5
#define  DEBUG_USART5_APBxClkCmd         RCC_APB1PeriphClockCmd

// USART5 GPIO ���ź궨��
#define  DEBUG_USART5_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
#define  DEBUG_USART5_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART5_TX_GPIO_PORT       GPIOC   
#define  DEBUG_USART5_TX_GPIO_PIN        GPIO_Pin_12
#define  DEBUG_USART5_RX_GPIO_PORT       GPIOD
#define  DEBUG_USART5_RX_GPIO_PIN        GPIO_Pin_2

#define  DEBUG_USART5_IRQ                UART5_IRQn
#define  DEBUG_USART5_IRQHandler         UART5_IRQHandler
#endif


/********************************������غ���*******************************/

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



