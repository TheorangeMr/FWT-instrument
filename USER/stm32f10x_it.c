/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <stdio.h>
#include "bsp_sdio_sdcard.h"
#include "bsp_can.h"




/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	printf("HardFault \r\n");
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//extern void xPortSysTickHandler( void );
///* 系统时钟服务函数 */
//void SysTick_Handler(void)
//{
//	#if (INCLUDE_xTaskGetSchedulerState  == 1 )
//		if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
//		{
//	#endif  /* INCLUDE_xTaskGetSchedulerState */  
//			xPortSysTickHandler();
//	#if (INCLUDE_xTaskGetSchedulerState  == 1 )
//		}
//	#endif  /* INCLUDE_xTaskGetSchedulerState */
//}


void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();  //SDIO中断处理函数
}

extern uint8_t RTC_Time_flag;
extern vu8 CAN_Recive_Flag;  
CanRxMsg RxMessage;                      //CAN接收信息结构体
extern vu8 VCU_Status1_Receive[8];       //VCU_Status1接收报文数组
extern vu8 VCU_Status2_Receive[8];       //VCU_Status2接收报文数组
uint8_t CanTx_Buf1[8] = {0};									//CAN发送缓存数组1
#define CAN_time_sure      0xab



/* CAN中断服务函数 */
void CAN_RX_IRQHandler(void)
{
	u8 i = 0;
	/* 初始化CAN接收器 */
	RxMessage.StdId=0x00;
  RxMessage.ExtId=0x00;
  RxMessage.IDE=0;
  RxMessage.DLC=0;
  RxMessage.FMI=0;
	
	for(i = 0; i < 8; i++)
	{
		RxMessage.Data[i]=0x00;
	}

	/* 接收数据 */
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

	/* 判断是否为目标方发送,并对数据进行校验 */
  if((RxMessage.ExtId==0x1998) && (RxMessage.IDE==CAN_ID_EXT) && (RxMessage.DLC==8)
		 && (RxMessage.Data[0]==0x02) && (RxMessage.Data[7] == 0x12))
  {
    CAN_Recive_Flag = 1;
		for(i = 0; i < 8; i++) {VCU_Status1_Receive[i] = RxMessage.Data[i];}
  }
	else if((RxMessage.ExtId==0x2001)&&(RxMessage.DLC==8)&&(RxMessage.IDE==CAN_ID_EXT)&&(RxMessage.Data[0]==0x02)
		&&(RxMessage.Data[7]==0x18))
	{
		printf("CanTx_Buf2 \r\n");
		CAN_Recive_Flag = 1;
		if(RTC_Time_flag <= 2)
		{
			RTC_Time_flag = 2;	
			CanTx_Buf1[0] = 0x02;                     
			CanTx_Buf1[1] = CAN_time_sure;
			CanTx_Buf1[2] = 0x18;
			CAN_SendMsg(0x2001 ,CanTx_Buf1, 3);                     //发送数据，确认已接受时间数据
		}
		for(i = 0; i < 8; i++)
		{
			VCU_Status2_Receive[i] = RxMessage.Data[i];
//			printf("%d\r\n",VCU_Status2_Receive[i]);
		}
	}
  else
  {
    CAN_Recive_Flag = 0; 
  }
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
