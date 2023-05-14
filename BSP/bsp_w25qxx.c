/*******************************************
	*�ļ��� ��  bsp_w25qxx.c
	*��   �ߣ�  ����
	*�޸�ʱ�䣺 2021.02.20
	*��   ����  v1.0
    *˵   ����  W25QXXоƬ����Դ�ļ�
*******************************************/

#include "bsp_w25qxx.h"
#include "bsp_delay.h"
#include "bsp_spi.h"
#include "string.h"


u16 W25QXX_TYPE;
u8 Sector_Buffer[4096];

/*******************************************
�������� W25QXX_Init()
��  �ܣ� �� W25QXXоƬ��ʼ��
��  ���� ��
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.2
********************************************/  
void W25QXX_Init(void)
{
	GPIO_InitTypeDef GPIO_Initstructure;

	W25QXX_CS_GPIO_APBxClock_FUN(W25QXX_CS_GPIO_APBxClock, ENABLE);   //ʹ��Ƭѡ����ʱ��

	MySPI_Init();                                                     //��ʼ��SPI

	/* Ƭѡ�������� */
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin = W25QXX_CS_PIN;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(W25QXX_CS_PORT, &GPIO_Initstructure);

	GPIO_SetBits(W25QXX_CS_PORT, W25QXX_CS_PIN);                       //��ʼ������ߵ�ƽ��ȡ��Ƭѡ��

	W25QXX_TYPE  = W25QXX_ReadID();                                    //��ȡоƬID��
}


/*******************************************
�������� W25QXX_ReadID()
��  �ܣ� ��ȡW25QXXоƬID 
��  ���� ��
����ֵ�� 16λID��ַ
��  �ߣ� ����
��  �ڣ� 2020.5.2
********************************************/  
u16  W25QXX_ReadID(void)
{
	u16 ID=0;
	Flash_CS_L;                                                  //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_ManufactDeviceID);                    //���Ͷ�����IDָ��
	
	/* ��24λ��ַ000000H�͵�оƬ��оƬ�ͻ�ͨ��DO���ء�����ID���� ������ID��*/
	SPI_ReadWriteByte(0);                   
	SPI_ReadWriteByte(0);
	SPI_ReadWriteByte(0);	

	//��ʼ��������ID
	ID=SPI_ReadWriteByte(0xff)<<8;
	ID+=SPI_ReadWriteByte(0xff);

	Flash_CS_H;                                                   //Ƭѡ����
	return ID;
}


/*******************************************
�������� W25QXX_ReadSR()
��  �ܣ� ��ȡ״̬�Ĵ���
��  ���� ��
����ֵ��״̬�Ĵ������ֽ����� 
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
u8 W25QXX_ReadSR(void)
{
	u8 StateWord;
    
	Flash_CS_L;                                                  //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_ReadStatusReg);                       //���Ͷ�״̬�Ĵ���ָ��
	StateWord = SPI_ReadWriteByte(0xff);                         //����״̬�Ĵ�������
	Flash_CS_H;                                                  //Ƭѡ����
    
	return StateWord;
}	


/*******************************************
�������� W25QXX_Write_SR()
��  �ܣ� ��һ���ֽ�����д��W25QXX״̬�Ĵ���
��  ���� SRdata
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25QXX_Write_SR(u8 SRdata)
{
	W25QXX_Write_Enable();                                       //дʹ��
	Flash_CS_L;                                                  //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_WriteStatusReg);                      //���Ͷ�״̬�Ĵ���ָ��
	SPI_ReadWriteByte(SRdata);                                   //д������
	Flash_CS_H;                                                  //Ƭѡ����
	W25QXX_Wait_Busy();					   		                 //�ȴ�д�����
}


/*******************************************
�������� W25QXX_Write_Enable()
��  �ܣ� W25QXXоƬдʹ��
��  ���� ��
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25QXX_Write_Enable(void)
{
	Flash_CS_L;                                                 //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_WriteEnable);                        //����дʹ��ָ��
	Flash_CS_H;                                                 //Ƭѡ����
}	


/*******************************************
�������� W25QXX_Write_Disable()
��  �ܣ� W25QXXоƬд����
��  ���� ��
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25QXX_Write_Disable(void)
{
	Flash_CS_L;                                                //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_WriteDisable);                      //����д����ָ��
	Flash_CS_H;                                                //Ƭѡ����
}


/*******************************************
�������� W25Qxx_Sector_Copy()
��  �ܣ� W25Qxx�������и��Ƶ�Sector_Buffer���飬���Ѵ�д�����ݸ���
��  ���� ����DataBuffer����ַWriteAddr���ֽ���Bytenum
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25Qxx_Sector_Copy(u8 *DataBuffer,u32 WriteAddr,u16 Bytenum)
{
	u16 i=0;
	u32 sector_addr,sector_offset;
	sector_addr=WriteAddr/4096;                                   //����ַת��Ϊ������ַ
	sector_offset=WriteAddr%4096;                                 //���������ڵ�ƫ����
	memset(Sector_Buffer,0xff,4096);                              //��Sector_Buffer����ȫ����ʼ��Ϊ0xff
	
	//���Ҫд���ֽڸ�������һ��������4096���������ݶ�ȡ��buffer������û��Ҫ��ȡ
	if(Bytenum<4096){
		W25QXX_Fast_Read(Sector_Buffer,sector_addr*4096,4096);    //����д�����������ȫ����ȡ
	}
	
	//������д�����ݸ��Ƶ�Sector_Buffer����
	while(i<Bytenum){
		Sector_Buffer[i+sector_offset]=DataBuffer[i];
		i++;
	}
	W25QXX_Erase_Sector(sector_addr*4096);                          //�߳�����
}


/*******************************************
�������� W25QXX_Read()
��  �ܣ� ��ȡW25QXX������
��  ���� DataBuffer���飬24λ��оƬ��ַReadAddr��Ҫ��ȡ�ֽ���Bytenum
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25QXX_Read(u8* DataBuffer,u32 ReadAddr,u16 Bytenum)
{
	u16 i=0;
	Flash_CS_L;                                                  //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_ReadData);                            //���Ͷ�����ָ��
	
	//����24λоƬ��ַ
	SPI_ReadWriteByte((ReadAddr&0xffffff)>>16);
	SPI_ReadWriteByte((ReadAddr&0xffff)>>8);
	SPI_ReadWriteByte(ReadAddr&0xff);
	
	//ѭ����ȡ����
	while(i<Bytenum){
		DataBuffer[i++]=SPI_ReadWriteByte(0xff);
	}
	Flash_CS_H;                                                  //Ƭѡ����
}


/*******************************************
�������� W25QXX_Fast_Read()
��  �ܣ� ���ٶ�ȡW25QXX������
��  ���� DataBuffer���飬24λ��оƬ��ַReadAddr��Ҫ��ȡ�ֽ���Bytenum
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25QXX_Fast_Read(u8* DataBuffer,u32 ReadAddr,u16 Bytenum)
{
	u16 i=0;
	Flash_CS_L;                                                 //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_FastReadData);                       //���Ͷ�����ָ��
	
	//����24λоƬ��ַ+һ���ն�д����
	SPI_ReadWriteByte((ReadAddr&0xffffff)>>16);
	SPI_ReadWriteByte((ReadAddr&0xffff)>>8);
	SPI_ReadWriteByte(ReadAddr&0xff);
	SPI_ReadWriteByte(0xff);
	
	//ѭ����ȡ����
	while(i<Bytenum){
		DataBuffer[i++]=SPI_ReadWriteByte(0xff);
	}
	Flash_CS_H;      
}


/*******************************************
�������� W25QXX_Write()
��  �ܣ� ������д��W25QXX
��  ���� DataBuffer���飬24λ��оƬ��ַWriteAddr��Ҫд���ֽ���Bytenum
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25QXX_Write(u8* DataBuffer,u32 WriteAddr,u16 Bytenum)
{
	u32 sector_addr;
	u16 sector_offset,sector_remainder;
	sector_addr=WriteAddr/4096;                                          //����ַת��Ϊ������ַ
	sector_offset=WriteAddr%4096;                                        //���������ڵ�ƫ����
	sector_remainder=4096-sector_offset;                                 //��������ʣ��ռ�
	
	//���Ҫд���ֽڸ���С�ڵ�������ʣ��ռ�
	if(Bytenum<=sector_remainder){
		W25Qxx_Sector_Copy(DataBuffer,WriteAddr,Bytenum);                //�����ݸ��Ƶ�Sector_Buffer��
		W25QXX_Write_Sector(Sector_Buffer,sector_addr);                  //��Sector_Buffer������д������
	}
	else{
		W25Qxx_Sector_Copy(DataBuffer,WriteAddr,sector_remainder);
		W25QXX_Write_Sector(Sector_Buffer,sector_addr);
		Bytenum-=sector_remainder;                                        //��д���ֽ�����sector_remainder
		WriteAddr+=sector_remainder;                                      //�ֽڵ�ַ��sector_remainder
		DataBuffer+=sector_remainder;                                     //����ָ���sector_remainder
		sector_addr++;                                                    //������ַ��1
		
		//���Bytenum����4096�������ȫ����д��
		while(Bytenum>4096){
			W25Qxx_Sector_Copy(DataBuffer,WriteAddr,4096);
		    W25QXX_Write_Sector(Sector_Buffer,sector_addr);
			Bytenum-=4096;                                                 //��д���ֽ�����4096
			WriteAddr+=4096;                                               //�ֽڵ�ַ��4096
			DataBuffer+=4096;                                              //����ָ���4096
			sector_addr++;                                                 //������ַ��1
		}
		
		//���ʣ�������Ϊ0
		if(Bytenum>0){
			W25Qxx_Sector_Copy(DataBuffer,WriteAddr,Bytenum);
		    W25QXX_Write_Sector(Sector_Buffer,sector_addr);
		}
	}
}


/*******************************************
�������� W25QXX_Write_Sector()
��  �ܣ� ��һ����������д��W25QXX
��  ���� ������ַSector_Addr
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25QXX_Write_Sector(u8 *DataBuffer,u32 Sector_Addr)
{
	u8 i=0;
	u32 address;
	address=Sector_Addr*4096;                            //������ַת��Ϊ�ֽڵ�ַ
	
	//һ������16ҳ
	while(i<16){
		W25QXX_Write_Page(DataBuffer,address,256);
		address+=256;                                    //�ֽڵ�ַ��256
		DataBuffer+=256;                                 //����ָ�������ƶ�256�������ָ�������ֵ��ѧϰ˼��
		i++;
	}
}


/*******************************************
�������� W25QXX_Write_Page()
��  �ܣ� ��һҳ����д��W25QXX
��  ���� DataBuffer���飬24λ��ҳ��ַ��ַWriteAddr��Ҫд���ֽ���Bytenum(���256)
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25QXX_Write_Page(u8* DataBuffer,u32 WriteAddr,u16 Bytenum)
{
	u16 i=0;
	W25QXX_Write_Enable();
	
	Flash_CS_L;                                             //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_PageProgram);                    //����ҳ���ָ��
	
	//����24λоƬ��ַ
	SPI_ReadWriteByte((WriteAddr&0xffffff)>>16);
	SPI_ReadWriteByte((WriteAddr&0xffff)>>8);
	SPI_ReadWriteByte(WriteAddr&0xff);
	
	//ѭ��д������
	while(i<Bytenum){
		SPI_ReadWriteByte(DataBuffer[i++]);
	}
	Flash_CS_H;                                            //Ƭѡ����
	W25QXX_Wait_Busy();					   		           //�ȴ�д�����
}



/*******************************************
�������� W25QXX_Erase_Chip()
��  �ܣ� ��W25QXX����ȫƬ�߳�
��  ���� ��
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25QXX_Erase_Chip(void)
{
	W25QXX_Write_Enable();                                //дʹ��
	Flash_CS_L;                                           //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_ChipErase);                    //����ȫƬ�߳�ָ��
	Flash_CS_H;                                           //Ƭѡ����
	W25QXX_Wait_Busy();					   		          //�ȴ�оƬ�߳�����
}


/*******************************************
�������� W25QXX_Erase_Block()
��  �ܣ� ��W25QXX���п��߳�
��  ���� ���ַBlock_Addr
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25QXX_Erase_Block(u32 Block_Addr)
{
	W25QXX_Write_Enable();                               //дʹ��
	Flash_CS_L;                                          //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_BlockErase);                  //���Ϳ��߳�ָ��
	
	//����24λ������ַ
	SPI_ReadWriteByte((Block_Addr&0xffffff)>>16);
	SPI_ReadWriteByte((Block_Addr&0xffff)>>8);
	SPI_ReadWriteByte(Block_Addr&0xff);
	
	Flash_CS_H;                                          //Ƭѡ����
	W25QXX_Wait_Busy();					   		         //�ȴ����߳�����
}



/*******************************************
�������� W25QXX_Erase_Sector()
��  �ܣ� ��W25QXX���������߳�
��  ���� ������ַSector_Addr
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25QXX_Erase_Sector(u32 Sector_Addr)
{
	W25QXX_Write_Enable();                              //дʹ��
	Flash_CS_L;                                         //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_SectorErase);                //���������߳�ָ��
	
	//����24λ������ַ
	SPI_ReadWriteByte((Sector_Addr&0xffffff)>>16);
	SPI_ReadWriteByte((Sector_Addr&0xffff)>>8);
	SPI_ReadWriteByte(Sector_Addr&0xff);
	
	Flash_CS_H;                                         //Ƭѡ����
	W25QXX_Wait_Busy();					   		        //�ȴ������߳�����
}


/*******************************************
�������� W25QXX_Wait_Busy()
��  �ܣ� ���оƬ�Ƿ�æ
��  ���� ��
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/ 
void W25QXX_Wait_Busy(void)
{
	while(W25QXX_ReadSR()&0x01);                        //�ȴ�BUSYλ����
}


/*******************************************
�������� W25QXX_PowerDown()
��  �ܣ� ��оƬ�������ģʽ
��  ���� ��
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.5.4
********************************************/  
void W25QXX_PowerDown(void)
{
	Flash_CS_L;                                       //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_PowerDown);                //���͵���ָ��
	Flash_CS_H;  
	Delay_Us(3);                                      //�ȴ�TPD  
}


/*******************************************
�������� W25QXX_WAKEUP()
��  �ܣ� ��оƬ�������ģʽ 
��  ���� ��
����ֵ�� ��
��  �ߣ� ����
��  �ڣ� 2020.4.25
********************************************/  
void W25QXX_WAKEUP(void)
{
	Flash_CS_L;                                     //Ƭѡʹ��
	SPI_ReadWriteByte(W25X_ReleasePowerDown);       //�����ͷŵ���ָ��
	Flash_CS_H;  
	Delay_Us(3);                                    //�ȴ�TRES1
}


/*
	*��������GB2312_Addr_Tans()
	*��  �ܣ����ַ�������ת��ΪW25QXX�ڲ��ֿ��ַ
	*��  ���� code���ַ�������,font������
	*����ֵ�� GT30L32S4W�ڲ���ַ,����0xffffffff��˵���������
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.07
*/
u32  GB2312_Addr_Tans(u8* code, FONT font)
{
  u32 addr = 0xffffffff;
	
  //��������ת������λ����ת�ɵ�ַƫ��
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
	*��������ASCII_Addr_Tans()
	*��  �ܣ����ַ�ASCII��ת��ΪW25QXX�ڲ��ֿ��ַ
	*��  ���� code���ַ�ASCII��,font������
	*����ֵ�� GT30L32S4W�ڲ���ַ,����0xffffffff��˵���������
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.07
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

