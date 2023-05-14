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
#define DEV_SD_CARD     0   /* SD卡设备号为0 */
#define DEV_SPI_FLASH   1   /* SPI FLASH 设备号为 1 */

extern SD_CardInfo SDCardInfo;    //SD卡信息存储结构体


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;       //设置设备初始状态为未被初始化
     u16 W25QXX_ID = 0xffff;

	switch (pdrv) {
	case DEV_SD_CARD :
        
        /* 如果SD卡初始化成功 */
        if(SD_Init() == SD_OK)
        {
            stat &= ~STA_NOINIT;    //返回值中的STA_NOINIT标志被清除
        }

		return stat;

	case DEV_SPI_FLASH :
        
        W25QXX_ID = W25QXX_ReadID();
        
        /* 如果W25QXXID读取匹配则初始化成功 */
        if(W25Q08==W25QXX_ID || W25Q16==W25QXX_ID || W25Q32==W25QXX_ID || W25Q64==W25QXX_ID || W25Q128==W25QXX_ID)
        {
            stat &= ~STA_NOINIT;   //返回值中的STA_NOINIT标志被清除
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
		return disk_status(DEV_SD_CARD);          //获取芯片状态

	case DEV_SPI_FLASH :
        W25QXX_Init();                            //初始化W25QXX芯片
		return disk_status(DEV_SPI_FLASH);        //获取芯片状态
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
        
        /* 如果内存地址不对齐 */
        if(((DWORD)buff&3) != 0)    //等价于buff%4 != 0
        {
            __align(4) u8 tempbuff[SDCardInfo.CardBlockSize];  //定义一个地址以4个字节对齐的临时buff
            
            //循环读取每一块的数据
            while(count--)
            {
                /* 单块读取数据到tempbuff(地址以4个字节对齐的临时buff) */
                sta = SD_ReadBlock(tempbuff, sector*SDCardInfo.CardBlockSize, SDCardInfo.CardBlockSize);
                sta = SD_WaitReadOperation();              //等待读操作完成
                while(SD_GetStatus() != SD_TRANSFER_OK);   //等待传输完成
                if(sta == SD_OK) //读取成功
                {
                    memcpy(buff, tempbuff, SDCardInfo.CardBlockSize); //将读取数据拷贝到buff中
                    res = RES_OK;
                }
                else
                {
                    res = RES_ERROR;
                }
                sector++;                              //扇区（块）地址加1
                buff += SDCardInfo.CardBlockSize;      //buff向后偏移SDCardInfo.CardBlockSize准备存储下一块读取的数据
            }
            
        }
        else //如果内存地址对齐
        {
            /* 直接多块读取数据到buff中 */
            sta = SD_ReadMultiBlocks((u8*)buff, sector*SDCardInfo.CardBlockSize, SDCardInfo.CardBlockSize, count);
            sta = SD_WaitReadOperation();             //等待读操作完成
            while(SD_GetStatus() != SD_TRANSFER_OK);  //等待传输完成
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
        sector += 256;                             //偏移1MB避开字库存储区
		W25QXX_Read(buff, sector<<12, count<<12);  //将扇区地址转换位字节地址进行读取数据（一个扇区4096字节）
        res = RES_OK;                              //默认SPI FLASH读取成功
		break;
    
    default:
        res = RES_PARERR;                          //没有设备号匹配，则返回错误信息为参数错误
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
        
        /* 内存地址不对齐 */
        if(((DWORD)buff&3) != 0)
        {
            __align(4) u8 tempbuff[SDCardInfo.CardBlockSize]; //定义一个地址以4个字节对齐的临时buff
            
            /* 循环写入多块数据 */
            while(count--)
            {
                memcpy(tempbuff, buff, SDCardInfo.CardBlockSize); //将buff中的数据拷贝到tempbuff中
                
                /* 将tempbuff中的数据写入SD卡中 */
                sta = SD_WriteBlock(tempbuff, sector*SDCardInfo.CardBlockSize, SDCardInfo.CardBlockSize);
                sta = SD_WaitWriteOperation();           //等待写操作完成
                while(SD_GetStatus() != SD_TRANSFER_OK); //等待传输完成
                if(sta == SD_OK) 
                {
                    res = RES_OK;
                }
                else
                {
                    res = RES_ERROR;
                }
                sector++;                                //扇区（块）地址加1
                buff += SDCardInfo.CardBlockSize;        //buff向后偏移SDCardInfo.CardBlockSize准备写入下一块的数据
            }
        }
        else //如果内存对齐
        {
            /* 直接将buff中的数据写入SD卡中 */
            sta = SD_WriteMultiBlocks((u8*)buff, sector*SDCardInfo.CardBlockSize, SDCardInfo.CardBlockSize, count);
            sta = SD_WaitWriteOperation();           //等待写操作完成
            while(SD_GetStatus() != SD_TRANSFER_OK); //等待传输完成
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
        sector += 256;                                   //偏移1MB避开字库存储区
        W25QXX_Write((u8*)buff, sector<<12, count<<12);  //将扇区地址转换位字节地址进行写入数据
        res = RES_OK;                                    //默认写入正常
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
            case GET_SECTOR_SIZE:  //获取扇区大小
                *(WORD*)buff = SDCardInfo.CardBlockSize;       
                break;
            
            case GET_BLOCK_SIZE:   //最小擦除单元的扇区数
                *(DWORD*)buff = 1;                            
                break;
            
            case GET_SECTOR_COUNT: //获取芯片扇区总数
                *(WORD*)buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;                           
                break;
        }
        res = RES_OK;
		break;

	case DEV_SPI_FLASH :

		//处理命令参数
        switch(cmd)
        {
            case GET_SECTOR_SIZE:          //获取扇区大小
                *(WORD*)buff = 4096;       //W25QXX系列扇区大小为4096字节
                break;
            
            case GET_BLOCK_SIZE:           //最小擦除单元的扇区数
                *(DWORD*)buff = 1;         //最小擦除单元为一个扇区
                break;
            
            case GET_SECTOR_COUNT:         //获取芯片扇区总数
                *(WORD*)buff = 4096;       //W25Q128共有256块（一个块有16个扇区）
                break;
        }
        
        res = RES_OK;
		break;
        
    default:
        res = RES_PARERR;                  //参数错误
        break;
            
	}

	return res;
}


/*-----------------------------------------------------------------------*/
/* 获取时间戳函数                                                         */
/*-----------------------------------------------------------------------*/
DWORD get_fattime (void)
{
    return 0;
}

