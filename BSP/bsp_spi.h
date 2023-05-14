/*******************************************
	*�ļ��� ��  bsp_spi.h
	*��   �ߣ�  ����
	*�޸�ʱ�䣺 2021.02.20
	*��   ����  v1.0
    *˵   ����  Ӳ��SPI����ͷ�ļ�
*******************************************/

/*
    SPI��Ƭѡ�������÷ŵ������Ӳ��������ʵ��
*/



#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f10x.h"


/************************************** SPI������غ궨�� *****************************************/
#define        USER_SPI_X                           SPI2                              //SPIXѡ��
#define        USER_SPI_APBX_CLOCK_FUN              RCC_APB1PeriphClockCmd            //SPIʱ��ʹ�ܺ���
#define        USER_SPI_APBX_CLOCK                  (RCC_APB1Periph_SPI2)             //SPIʱ��
#define        USER_SPI_GPIO_APBxClock_FUN          RCC_APB2PeriphClockCmd            //SPI����ʹ�ܺ���
#define        USER_SPI_GPIO_APBxClock              (RCC_APB2Periph_GPIOB)            //GPIOʱ��

#define        USER_SPI_CLK_PORT                    GPIOB                             //SPIʱ��
#define        USER_SPI_CLK_PIN                     GPIO_Pin_13

#define        USER_SPI_MISO_PORT                   GPIOB                             //SPI��MISO
#define        USER_SPI_MISO_PIN                    GPIO_Pin_14

#define        USER_SPI_MOSI_PORT                   GPIOB                             //SPI��MOSI
#define        USER_SPI_MOSI_PIN                    GPIO_Pin_15


/*****************************************SPI��ع��ܺ���******************************************/

void MySPI_Init(void);
u8 SPI_ReadWriteByte(u8 zdata);
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler);


#endif
	

