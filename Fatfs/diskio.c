/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "bsp_w25qxx.h"
#include "bsp_sdio_sdcard.h"
#include <string.h>

/* Definitions of physical drive number for each drive */
//#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define DEV_SD_CARD     0   /* SD���豸��Ϊ0 */
#define DEV_SPI_FLASH   1   /* SPI FLASH �豸��Ϊ 1 */

extern SD_CardInfo SDCardInfo;    //SD����Ϣ�洢�ṹ��


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;       //�����豸��ʼ״̬Ϊδ����ʼ��
     u16 W25QXX_ID = 0xffff;

	switch (pdrv) {
	case DEV_SD_CARD :
        
        /* ���SD����ʼ���ɹ� */
        if(SD_Init() == SD_OK)
        {
            stat &= ~STA_NOINIT;    //����ֵ�е�STA_NOINIT��־�����
        }

		return stat;

	case DEV_SPI_FLASH :
        
        W25QXX_ID = W25QXX_ReadID();
        
        /* ���W25QXXID��ȡƥ�����ʼ���ɹ� */
        if(W25Q08==W25QXX_ID || W25Q16==W25QXX_ID || W25Q32==W25QXX_ID || W25Q64==W25QXX_ID || W25Q128==W25QXX_ID)
        {
            stat &= ~STA_NOINIT;   //����ֵ�е�STA_NOINIT��־�����
        }
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	switch (pdrv) {
	case DEV_SD_CARD :
		return disk_status(DEV_SD_CARD);          //��ȡоƬ״̬

	case DEV_SPI_FLASH :
        W25QXX_Init();                            //��ʼ��W25QXXоƬ
		return disk_status(DEV_SPI_FLASH);        //��ȡоƬ״̬
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_PARERR;
    SD_Error sta;

	switch (pdrv) {
	case DEV_SD_CARD :
        
        /* ����ڴ��ַ������ */
        if(((DWORD)buff&3) != 0)    //�ȼ���buff%4 != 0
        {
            __align(4) u8 tempbuff[SDCardInfo.CardBlockSize];  //����һ����ַ��4���ֽڶ������ʱbuff
            
            //ѭ����ȡÿһ�������
            while(count--)
            {
                /* �����ȡ���ݵ�tempbuff(��ַ��4���ֽڶ������ʱbuff) */
                sta = SD_ReadBlock(tempbuff, sector*SDCardInfo.CardBlockSize, SDCardInfo.CardBlockSize);
                sta = SD_WaitReadOperation();              //�ȴ����������
                while(SD_GetStatus() != SD_TRANSFER_OK);   //�ȴ��������
                if(sta == SD_OK) //��ȡ�ɹ�
                {
                    memcpy(buff, tempbuff, SDCardInfo.CardBlockSize); //����ȡ���ݿ�����buff��
                    res = RES_OK;
                }
                else
                {
                    res = RES_ERROR;
                }
                sector++;                              //�������飩��ַ��1
                buff += SDCardInfo.CardBlockSize;      //buff���ƫ��SDCardInfo.CardBlockSize׼���洢��һ���ȡ������
            }
            
        }
        else //����ڴ��ַ����
        {
            /* ֱ�Ӷ���ȡ���ݵ�buff�� */
            sta = SD_ReadMultiBlocks((u8*)buff, sector*SDCardInfo.CardBlockSize, SDCardInfo.CardBlockSize, count);
            sta = SD_WaitReadOperation();             //�ȴ����������
            while(SD_GetStatus() != SD_TRANSFER_OK);  //�ȴ��������
            if(sta == SD_OK)
            {
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
        }
		break;

	case DEV_SPI_FLASH :
        sector += 256;                             //ƫ��1MB�ܿ��ֿ�洢��
		W25QXX_Read(buff, sector<<12, count<<12);  //��������ַת��λ�ֽڵ�ַ���ж�ȡ���ݣ�һ������4096�ֽڣ�
        res = RES_OK;                              //Ĭ��SPI FLASH��ȡ�ɹ�
		break;
    
    default:
        res = RES_PARERR;                          //û���豸��ƥ�䣬�򷵻ش�����ϢΪ��������
        break;
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
    SD_Error sta;

	switch (pdrv) {
	case DEV_SD_CARD :
        
        /* �ڴ��ַ������ */
        if(((DWORD)buff&3) != 0)
        {
            __align(4) u8 tempbuff[SDCardInfo.CardBlockSize]; //����һ����ַ��4���ֽڶ������ʱbuff
            
            /* ѭ��д�������� */
            while(count--)
            {
                memcpy(tempbuff, buff, SDCardInfo.CardBlockSize); //��buff�е����ݿ�����tempbuff��
                
                /* ��tempbuff�е�����д��SD���� */
                sta = SD_WriteBlock(tempbuff, sector*SDCardInfo.CardBlockSize, SDCardInfo.CardBlockSize);
                sta = SD_WaitWriteOperation();           //�ȴ�д�������
                while(SD_GetStatus() != SD_TRANSFER_OK); //�ȴ��������
                if(sta == SD_OK) 
                {
                    res = RES_OK;
                }
                else
                {
                    res = RES_ERROR;
                }
                sector++;                                //�������飩��ַ��1
                buff += SDCardInfo.CardBlockSize;        //buff���ƫ��SDCardInfo.CardBlockSize׼��д����һ�������
            }
        }
        else //����ڴ����
        {
            /* ֱ�ӽ�buff�е�����д��SD���� */
            sta = SD_WriteMultiBlocks((u8*)buff, sector*SDCardInfo.CardBlockSize, SDCardInfo.CardBlockSize, count);
            sta = SD_WaitWriteOperation();           //�ȴ�д�������
            while(SD_GetStatus() != SD_TRANSFER_OK); //�ȴ��������
            if(sta == SD_OK)
            {
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
        }
		break;

	case DEV_SPI_FLASH :
        sector += 256;                                   //ƫ��1MB�ܿ��ֿ�洢��
        W25QXX_Write((u8*)buff, sector<<12, count<<12);  //��������ַת��λ�ֽڵ�ַ����д������
        res = RES_OK;                                    //Ĭ��д������
		break;
    
    default:
        res = RES_PARERR;
        break;
	}

	return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {
	case DEV_SD_CARD :

		// Process of the command for the RAM drive
        switch(cmd)
        {
            case GET_SECTOR_SIZE:  //��ȡ������С
                *(WORD*)buff = SDCardInfo.CardBlockSize;       
                break;
            
            case GET_BLOCK_SIZE:   //��С������Ԫ��������
                *(DWORD*)buff = 1;                            
                break;
            
            case GET_SECTOR_COUNT: //��ȡоƬ��������
                *(WORD*)buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;                           
                break;
        }
        res = RES_OK;
		break;

	case DEV_SPI_FLASH :

		//�����������
        switch(cmd)
        {
            case GET_SECTOR_SIZE:          //��ȡ������С
                *(WORD*)buff = 4096;       //W25QXXϵ��������СΪ4096�ֽ�
                break;
            
            case GET_BLOCK_SIZE:           //��С������Ԫ��������
                *(DWORD*)buff = 1;         //��С������ԪΪһ������
                break;
            
            case GET_SECTOR_COUNT:         //��ȡоƬ��������
                *(WORD*)buff = 4096;       //W25Q128����256�飨һ������16��������
                break;
        }
        
        res = RES_OK;
		break;
        
    default:
        res = RES_PARERR;                  //��������
        break;
            
	}

	return res;
}


/*-----------------------------------------------------------------------*/
/* ��ȡʱ�������                                                         */
/*-----------------------------------------------------------------------*/
DWORD get_fattime (void)
{
    return 0;
}

