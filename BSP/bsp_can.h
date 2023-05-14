#ifndef _BSP_CAN_H
#define _BSP_CAN_H

#include "stm32f10x.h"

/************************************************************
								CAN������ź�ʱ�Ӻ궨��
************************************************************/
#define CANx                       CAN1
#define CAN_CLK                    RCC_APB1Periph_CAN1
#define CAN_RX_IRQ								 USB_LP_CAN1_RX0_IRQn
#define CAN_RX_IRQHandler					 USB_LP_CAN1_RX0_IRQHandler

/* CAN RX����*/
#define CAN_RX_PORT                GPIOA
#define CAN_RX_CLK                 RCC_APB2Periph_GPIOA
#define CAN_RX_PIN                 GPIO_Pin_11

/* CAN TX���� */
#define CAN_TX_PORT                GPIOA
#define CAN_TX_CLK                 (RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA)
#define CAN_TX_PIN                 GPIO_Pin_12


/* ���䷵�ر�־ */
#define FAILED (0) //����ʧ��
#define PASSED (1) //����ɹ�




/*****************************************************************
								       CAN���⺯����
*****************************************************************/
void CAN_Config(void);
uint8_t CAN_SendMsg(u32 Ext_ID ,u8* zdata, u8 len);
uint8_t CAN_ReciveMsg(u8 *zdata, u8 len);

#endif /* !_BSP_CAN_H */


