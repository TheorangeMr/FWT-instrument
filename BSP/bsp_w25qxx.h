/*******************************************
	*�ļ��� ��  bsp_w25qxx.h
	*��   �ߣ�  ����
	*�޸�ʱ�䣺 2021.02.20
	*��   ����  v1.0
  *˵   ����  W25QXXоƬ����ͷ�ļ�
*******************************************/

#ifndef _BSP_W25QXX_H
#define _BSP_W25QXX_H
#include "sys.h"

/****************************�ֿ��ַ****************************/

//GB2312_16X16�ֿ�洢�׵�ַ 
#define GB2312_16x16_START_ADDR          (u32)0x000000
//GB2312_24X24�ֿ�洢�׵�ַ
#define GB2312_24x24_START_ADDR          (u32)(0x000000 + 65*4096)
//GB2312_32X32�ֿ�洢�׵�ַ
#define GB2312_32x32_START_ADDR          (u32)(0x000000 + 210*4096)
//ASCII_8X16�ֿ�洢�׵�ַ
#define ASCII_8x16_START_ADDR            (u32)(0x000000 + 466*4096)
//ASCII_12X24�ֿ�洢�׵�ַ
#define ASCII_12x24_START_ADDR           (u32)(0x000000 + 468*4096)
//ASCII_8X16�ֿ�洢�׵�ַ
#define ASCII_16x32_START_ADDR           (u32)(0x000000 + 474*4096)
//�Զ�������NewDigtal�ֿ��׵�ַ
#define NewDigtal_START_ADDR             (u32)(0x000000 + 476*4096)


/**************************Flash�ֿ�����ö��***********************/

typedef enum
{
	GB2312_16x16 = 0,
	GB2312_24x24,
	GB2312_32x32,
	ASCII_8X16,
	ASCII_12X24,
	ASCII_16X32,
}FONT;



/********************************W25Xϵ��/Qϵ��оƬ�б�********************************/	   
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

/****************************** W25Qϵ��оƬƬѡ���ź궨�� *****************************/
#define W25QXX_CS_GPIO_APBxClock_FUN          RCC_APB2PeriphClockCmd            //Ƭѡ����ʱ��ʹ�ܺ���
#define W25QXX_CS_GPIO_APBxClock              (RCC_APB2Periph_GPIOB)            //Ƭѡ����GPIOʱ��
#define W25QXX_CS_PORT                        GPIOB                             //Ƭѡ���Ŷ˿�
#define W25QXX_CS_PIN                         GPIO_Pin_12                       //Ƭѡ���ź�

#define Flash_CS_L                            GPIO_ResetBits(GPIOB,GPIO_Pin_12) //FlashƬѡʹ��
#define Flash_CS_H                            GPIO_SetBits(GPIOB,GPIO_Pin_12)   //FlashƬѡ����



/*********************************W25Qϵ��оƬָ���**********************************/
#define W25X_WriteEnable		                   0x06       //дʹ��
#define W25X_WriteDisable		                   0x04       //д����
#define W25X_ReadStatusReg		                 0x05       //��״̬�Ĵ���
#define W25X_WriteStatusReg		                 0x01       //д״̬�Ĵ���
#define W25X_ReadData			                     0x03       //������
#define W25X_FastReadData		                   0x0B       //���
#define W25X_FastReadDual		                   0x3B       //���˫���
#define W25X_PageProgram		                   0x02       //ҳ���
#define W25X_BlockErase			                   0xD8       //���߳�
#define W25X_SectorErase		                   0x20       //�����߳�
#define W25X_ChipErase			                   0xC7       //ȫоƬ�߳�
#define W25X_PowerDown			                   0xB9       //����
#define W25X_ReleasePowerDown	                 0xAB       //�ͷŵ���
#define W25X_DeviceID			                     0xAB       //��������ID
#define W25X_ManufactDeviceID	                 0x90       
#define W25X_JedecDeviceID		                 0x9F       

/*************************************W25Qϵ��оƬ���ܺ���*********************************************/
void W25QXX_Init(void);                                                     //W25QXX��ʼ��
u16  W25QXX_ReadID(void);  	    		                                        //��ȡFLASH ID
u8	 W25QXX_ReadSR(void);        		                                        //��ȡ״̬�Ĵ��� 
void W25QXX_Write_SR(u8 SRdata);  			                                    //д״̬�Ĵ���
void W25QXX_Write_Enable(void);  		                                        //дʹ�� 
void W25QXX_Write_Disable(void);		                                        //д����
void W25Qxx_Sector_Copy(u8 *DataBuffer,u32 WriteAddr,u16 Bytenum);          //��������
void W25QXX_Read(u8* DataBuffer,u32 ReadAddr,u16 Bytenum);                  //��ȡflash
void W25QXX_Fast_Read(u8* DataBuffer,u32 ReadAddr,u16 Bytenum);             //���ٶ�ȡFLASH
void W25QXX_Write(u8* DataBuffer,u32 WriteAddr,u16 Bytenum);                //д������
void W25QXX_Write_Sector(u8 *DataBuffer,u32 Sector_Addr);                   //д��һ������������
void W25QXX_Write_Page(u8* DataBuffer,u32 WriteAddr,u16 Bytenum);           //д��һҳ����
void W25QXX_Erase_Chip(void);    	  	                                      //��Ƭ����
void W25QXX_Erase_Block(u32 Block_Addr);	                                  //��������
void W25QXX_Erase_Sector(u32 Sector_Addr);	                                //��������
void W25QXX_Wait_Busy(void);           	                                    //�ȴ�����
void W25QXX_PowerDown(void);        	                                      //�������ģʽ
void W25QXX_WAKEUP(void);	                                                  //����

/******************************������תת�ֿ��ַ����****************************/

u32  GB2312_Addr_Tans(u8* code, FONT font);
u32  ASCII_Addr_Tans(char code, FONT font);

#endif

