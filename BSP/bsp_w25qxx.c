/*******************************************
	*文件名 ：  bsp_w25qxx.c
	*作   者：  王峰
	*修改时间： 2021.02.20
	*版   本：  v1.0
    *说   明：  W25QXX芯片驱动源文件
*******************************************/

#include "bsp_w25qxx.h"
#include "bsp_delay.h"
#include "bsp_spi.h"
#include "string.h"


u16 W25QXX_TYPE;
u8 Sector_Buffer[4096];

/*******************************************
函数名： W25QXX_Init()
功  能： 对 W25QXX芯片初始化
参  数： 无
返回值： 无
作  者： 王峰
日  期： 2020.5.2
********************************************/  
void W25QXX_Init(void)
{
	GPIO_InitTypeDef GPIO_Initstructure;

	W25QXX_CS_GPIO_APBxClock_FUN(W25QXX_CS_GPIO_APBxClock, ENABLE);   //使能片选引脚时钟

	MySPI_Init();                                                     //初始化SPI

	/* 片选引脚配置 */
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin = W25QXX_CS_PIN;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(W25QXX_CS_PORT, &GPIO_Initstructure);

	GPIO_SetBits(W25QXX_CS_PORT, W25QXX_CS_PIN);                       //初始化输出高电平（取消片选）

	W25QXX_TYPE  = W25QXX_ReadID();                                    //读取芯片ID号
}


/*******************************************
函数名： W25QXX_ReadID()
功  能： 读取W25QXX芯片ID 
参  数： 无
返回值： 16位ID地址
作  者： 王峰
日  期： 2020.5.2
********************************************/  
u16  W25QXX_ReadID(void)
{
	u16 ID=0;
	Flash_CS_L;                                                  //片选使能
	SPI_ReadWriteByte(W25X_ManufactDeviceID);                    //发送读器件ID指令
	
	/* 将24位地址000000H送到芯片，芯片就会通过DO返回“生产ID”和 “器件ID”*/
	SPI_ReadWriteByte(0);                   
	SPI_ReadWriteByte(0);
	SPI_ReadWriteByte(0);	

	//开始接收器件ID
	ID=SPI_ReadWriteByte(0xff)<<8;
	ID+=SPI_ReadWriteByte(0xff);

	Flash_CS_H;                                                   //片选禁能
	return ID;
}


/*******************************************
函数名： W25QXX_ReadSR()
功  能： 读取状态寄存器
参  数： 无
返回值：状态寄存器的字节数据 
作  者： 王峰
日  期： 2020.5.4
********************************************/  
u8 W25QXX_ReadSR(void)
{
	u8 StateWord;
    
	Flash_CS_L;                                                  //片选使能
	SPI_ReadWriteByte(W25X_ReadStatusReg);                       //发送读状态寄存器指令
	StateWord = SPI_ReadWriteByte(0xff);                         //接收状态寄存器数据
	Flash_CS_H;                                                  //片选禁能
    
	return StateWord;
}	


/*******************************************
函数名： W25QXX_Write_SR()
功  能： 将一个字节数据写入W25QXX状态寄存器
参  数： SRdata
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25QXX_Write_SR(u8 SRdata)
{
	W25QXX_Write_Enable();                                       //写使能
	Flash_CS_L;                                                  //片选使能
	SPI_ReadWriteByte(W25X_WriteStatusReg);                      //发送读状态寄存器指令
	SPI_ReadWriteByte(SRdata);                                   //写入数据
	Flash_CS_H;                                                  //片选禁能
	W25QXX_Wait_Busy();					   		                 //等待写入结束
}


/*******************************************
函数名： W25QXX_Write_Enable()
功  能： W25QXX芯片写使能
参  数： 无
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25QXX_Write_Enable(void)
{
	Flash_CS_L;                                                 //片选使能
	SPI_ReadWriteByte(W25X_WriteEnable);                        //发送写使能指令
	Flash_CS_H;                                                 //片选禁能
}	


/*******************************************
函数名： W25QXX_Write_Disable()
功  能： W25QXX芯片写禁能
参  数： 无
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25QXX_Write_Disable(void)
{
	Flash_CS_L;                                                //片选使能
	SPI_ReadWriteByte(W25X_WriteDisable);                      //发送写禁能指令
	Flash_CS_H;                                                //片选禁能
}


/*******************************************
函数名： W25Qxx_Sector_Copy()
功  能： W25Qxx扇区进行复制到Sector_Buffer数组，并把待写入数据复制
参  数： 数组DataBuffer，地址WriteAddr，字节数Bytenum
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25Qxx_Sector_Copy(u8 *DataBuffer,u32 WriteAddr,u16 Bytenum)
{
	u16 i=0;
	u32 sector_addr,sector_offset;
	sector_addr=WriteAddr/4096;                                   //将地址转化为扇区地址
	sector_offset=WriteAddr%4096;                                 //计算扇区内的偏移量
	memset(Sector_Buffer,0xff,4096);                              //将Sector_Buffer数组全部初始化为0xff
	
	//如果要写入字节个数不到一个扇区（4096），则将数据读取至buffer，否则没必要读取
	if(Bytenum<4096){
		W25QXX_Fast_Read(Sector_Buffer,sector_addr*4096,4096);    //将待写入的扇区内容全部读取
	}
	
	//遍历待写入数据复制到Sector_Buffer数组
	while(i<Bytenum){
		Sector_Buffer[i+sector_offset]=DataBuffer[i];
		i++;
	}
	W25QXX_Erase_Sector(sector_addr*4096);                          //檫除扇区
}


/*******************************************
函数名： W25QXX_Read()
功  能： 读取W25QXX的数据
参  数： DataBuffer数组，24位的芯片地址ReadAddr，要读取字节数Bytenum
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25QXX_Read(u8* DataBuffer,u32 ReadAddr,u16 Bytenum)
{
	u16 i=0;
	Flash_CS_L;                                                  //片选使能
	SPI_ReadWriteByte(W25X_ReadData);                            //发送读数据指令
	
	//发送24位芯片地址
	SPI_ReadWriteByte((ReadAddr&0xffffff)>>16);
	SPI_ReadWriteByte((ReadAddr&0xffff)>>8);
	SPI_ReadWriteByte(ReadAddr&0xff);
	
	//循环读取数据
	while(i<Bytenum){
		DataBuffer[i++]=SPI_ReadWriteByte(0xff);
	}
	Flash_CS_H;                                                  //片选禁能
}


/*******************************************
函数名： W25QXX_Fast_Read()
功  能： 快速读取W25QXX的数据
参  数： DataBuffer数组，24位的芯片地址ReadAddr，要读取字节数Bytenum
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25QXX_Fast_Read(u8* DataBuffer,u32 ReadAddr,u16 Bytenum)
{
	u16 i=0;
	Flash_CS_L;                                                 //片选使能
	SPI_ReadWriteByte(W25X_FastReadData);                       //发送读数据指令
	
	//发送24位芯片地址+一个空读写周期
	SPI_ReadWriteByte((ReadAddr&0xffffff)>>16);
	SPI_ReadWriteByte((ReadAddr&0xffff)>>8);
	SPI_ReadWriteByte(ReadAddr&0xff);
	SPI_ReadWriteByte(0xff);
	
	//循环读取数据
	while(i<Bytenum){
		DataBuffer[i++]=SPI_ReadWriteByte(0xff);
	}
	Flash_CS_H;      
}


/*******************************************
函数名： W25QXX_Write()
功  能： 将数据写入W25QXX
参  数： DataBuffer数组，24位的芯片地址WriteAddr，要写入字节数Bytenum
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25QXX_Write(u8* DataBuffer,u32 WriteAddr,u16 Bytenum)
{
	u32 sector_addr;
	u16 sector_offset,sector_remainder;
	sector_addr=WriteAddr/4096;                                          //将地址转化为扇区地址
	sector_offset=WriteAddr%4096;                                        //计算扇区内的偏移量
	sector_remainder=4096-sector_offset;                                 //计算扇区剩余空间
	
	//如果要写入字节个数小于等于扇区剩余空间
	if(Bytenum<=sector_remainder){
		W25Qxx_Sector_Copy(DataBuffer,WriteAddr,Bytenum);                //将数据复制到Sector_Buffer中
		W25QXX_Write_Sector(Sector_Buffer,sector_addr);                  //将Sector_Buffer中数据写入扇区
	}
	else{
		W25Qxx_Sector_Copy(DataBuffer,WriteAddr,sector_remainder);
		W25QXX_Write_Sector(Sector_Buffer,sector_addr);
		Bytenum-=sector_remainder;                                        //待写入字节数减sector_remainder
		WriteAddr+=sector_remainder;                                      //字节地址加sector_remainder
		DataBuffer+=sector_remainder;                                     //数据指针加sector_remainder
		sector_addr++;                                                    //扇区地址加1
		
		//如果Bytenum大于4096，则进行全扇区写入
		while(Bytenum>4096){
			W25Qxx_Sector_Copy(DataBuffer,WriteAddr,4096);
		    W25QXX_Write_Sector(Sector_Buffer,sector_addr);
			Bytenum-=4096;                                                 //待写入字节数减4096
			WriteAddr+=4096;                                               //字节地址加4096
			DataBuffer+=4096;                                              //数据指针加4096
			sector_addr++;                                                 //扇区地址加1
		}
		
		//如果剩余个数不为0
		if(Bytenum>0){
			W25Qxx_Sector_Copy(DataBuffer,WriteAddr,Bytenum);
		    W25QXX_Write_Sector(Sector_Buffer,sector_addr);
		}
	}
}


/*******************************************
函数名： W25QXX_Write_Sector()
功  能： 将一个扇区数据写入W25QXX
参  数： 扇区地址Sector_Addr
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25QXX_Write_Sector(u8 *DataBuffer,u32 Sector_Addr)
{
	u8 i=0;
	u32 address;
	address=Sector_Addr*4096;                            //扇区地址转化为字节地址
	
	//一个扇区16页
	while(i<16){
		W25QXX_Write_Page(DataBuffer,address,256);
		address+=256;                                    //字节地址加256
		DataBuffer+=256;                                 //数据指针往后移动256，这里对指针的运用值得学习思考
		i++;
	}
}


/*******************************************
函数名： W25QXX_Write_Page()
功  能： 将一页数据写入W25QXX
参  数： DataBuffer数组，24位的页地址地址WriteAddr，要写入字节数Bytenum(最大256)
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25QXX_Write_Page(u8* DataBuffer,u32 WriteAddr,u16 Bytenum)
{
	u16 i=0;
	W25QXX_Write_Enable();
	
	Flash_CS_L;                                             //片选使能
	SPI_ReadWriteByte(W25X_PageProgram);                    //发送页编程指令
	
	//发送24位芯片地址
	SPI_ReadWriteByte((WriteAddr&0xffffff)>>16);
	SPI_ReadWriteByte((WriteAddr&0xffff)>>8);
	SPI_ReadWriteByte(WriteAddr&0xff);
	
	//循环写入数据
	while(i<Bytenum){
		SPI_ReadWriteByte(DataBuffer[i++]);
	}
	Flash_CS_H;                                            //片选禁能
	W25QXX_Wait_Busy();					   		           //等待写入结束
}



/*******************************************
函数名： W25QXX_Erase_Chip()
功  能： 对W25QXX进行全片檫除
参  数： 无
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25QXX_Erase_Chip(void)
{
	W25QXX_Write_Enable();                                //写使能
	Flash_CS_L;                                           //片选使能
	SPI_ReadWriteByte(W25X_ChipErase);                    //发送全片檫除指令
	Flash_CS_H;                                           //片选禁能
	W25QXX_Wait_Busy();					   		          //等待芯片檫除结束
}


/*******************************************
函数名： W25QXX_Erase_Block()
功  能： 对W25QXX进行块檫除
参  数： 块地址Block_Addr
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25QXX_Erase_Block(u32 Block_Addr)
{
	W25QXX_Write_Enable();                               //写使能
	Flash_CS_L;                                          //片选使能
	SPI_ReadWriteByte(W25X_BlockErase);                  //发送块檫除指令
	
	//发送24位块区地址
	SPI_ReadWriteByte((Block_Addr&0xffffff)>>16);
	SPI_ReadWriteByte((Block_Addr&0xffff)>>8);
	SPI_ReadWriteByte(Block_Addr&0xff);
	
	Flash_CS_H;                                          //片选禁能
	W25QXX_Wait_Busy();					   		         //等待块檫除结束
}



/*******************************************
函数名： W25QXX_Erase_Sector()
功  能： 对W25QXX进行扇区檫除
参  数： 扇区地址Sector_Addr
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25QXX_Erase_Sector(u32 Sector_Addr)
{
	W25QXX_Write_Enable();                              //写使能
	Flash_CS_L;                                         //片选使能
	SPI_ReadWriteByte(W25X_SectorErase);                //发送扇区檫除指令
	
	//发送24位扇区地址
	SPI_ReadWriteByte((Sector_Addr&0xffffff)>>16);
	SPI_ReadWriteByte((Sector_Addr&0xffff)>>8);
	SPI_ReadWriteByte(Sector_Addr&0xff);
	
	Flash_CS_H;                                         //片选禁能
	W25QXX_Wait_Busy();					   		        //等待扇区檫除结束
}


/*******************************************
函数名： W25QXX_Wait_Busy()
功  能： 检测芯片是否繁忙
参  数： 无
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/ 
void W25QXX_Wait_Busy(void)
{
	while(W25QXX_ReadSR()&0x01);                        //等待BUSY位清零
}


/*******************************************
函数名： W25QXX_PowerDown()
功  能： 让芯片进入掉电模式
参  数： 无
返回值： 无
作  者： 王峰
日  期： 2020.5.4
********************************************/  
void W25QXX_PowerDown(void)
{
	Flash_CS_L;                                       //片选使能
	SPI_ReadWriteByte(W25X_PowerDown);                //发送掉电指令
	Flash_CS_H;  
	Delay_Us(3);                                      //等待TPD  
}


/*******************************************
函数名： W25QXX_WAKEUP()
功  能： 让芯片解除掉电模式 
参  数： 无
返回值： 无
作  者： 王峰
日  期： 2020.4.25
********************************************/  
void W25QXX_WAKEUP(void)
{
	Flash_CS_L;                                     //片选使能
	SPI_ReadWriteByte(W25X_ReleasePowerDown);       //发送释放掉电指令
	Flash_CS_H;  
	Delay_Us(3);                                    //等待TRES1
}


/*
	*函数名：GB2312_Addr_Tans()
	*功  能：将字符机内码转换为W25QXX内部字库地址
	*参  数： code：字符机内码,font：字体
	*返回值： GT30L32S4W内部地址,返回0xffffffff则说明字体出错
	*作  者： 王峰
	*日  期： 2021.01.07
*/
u32  GB2312_Addr_Tans(u8* code, FONT font)
{
  u32 addr = 0xffffffff;
	
  //将机内码转化成区位码再转成地址偏移
	switch (font)
	{
		case GB2312_16x16:
			addr = GB2312_16x16_START_ADDR + ((code[0] - 0xa0 - 1)*94 +(code[1] - 0xa0 - 1))*32;
			break;
		case GB2312_24x24:
			addr = GB2312_24x24_START_ADDR + ((code[0] - 0xa0 - 1)*94 +(code[1] - 0xa0 - 1))*72;
			break;
		case GB2312_32x32:
			addr = GB2312_32x32_START_ADDR + ((code[0] - 0xa0 - 1)*94 +(code[1] - 0xa0 - 1))*128;
			break;
		default:
			addr = 0xffffffff;
			break;
	}
	
	return addr;
}


/*
	*函数名：ASCII_Addr_Tans()
	*功  能：将字符ASCII码转换为W25QXX内部字库地址
	*参  数： code：字符ASCII码,font：字体
	*返回值： GT30L32S4W内部地址,返回0xffffffff则说明字体出错
	*作  者： 王峰
	*日  期： 2021.01.07
*/
u32  ASCII_Addr_Tans(char code, FONT font)
{
  u32 addr = 0xffffffff;
	
	switch (font)
	{
		case ASCII_8X16:
			addr = ASCII_8x16_START_ADDR + (code - 32) * 16;
			break;
		case ASCII_12X24:
			addr = ASCII_12x24_START_ADDR + (code-32) * 48;
			break;
		case ASCII_16X32:
			addr = ASCII_16x32_START_ADDR + (code - 32) * 64;
			break;
		default:
			addr = 0xffffffff;
			break;
	}
	
	return addr;
}

