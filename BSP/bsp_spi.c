/*******************************************
	*文件名 ：  bsp_spi.c
	*作   者：  王峰
	*修改时间： 2021.02.20
	*版   本：  v1.0
    *说   明：  硬件SPI配置源文件
*******************************************/


#include "bsp_spi.h"



/*
	*函数名： MySPI_Init()
	*功  能： SPI初始化函数
	*参  数： 无
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.02.20
*/
void MySPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;
	
	//使能SPI和GPIO时钟
	USER_SPI_APBX_CLOCK_FUN(USER_SPI_APBX_CLOCK , ENABLE);
	USER_SPI_GPIO_APBxClock_FUN(USER_SPI_GPIO_APBxClock , ENABLE);
	
	/****配置引脚工作模式****/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//时钟引脚
	GPIO_InitStructure.GPIO_Pin = USER_SPI_CLK_PIN  ;
	GPIO_Init(USER_SPI_CLK_PORT , &GPIO_InitStructure);
	//MISO引脚
	GPIO_InitStructure.GPIO_Pin = USER_SPI_MISO_PIN  ;
	GPIO_Init(USER_SPI_MISO_PORT , &GPIO_InitStructure);
	//MOSI引脚
	GPIO_InitStructure.GPIO_Pin = USER_SPI_MOSI_PIN  ;
	GPIO_Init(USER_SPI_MOSI_PORT , &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);              //PB13/14/15上拉
	
	//配置SPI工作模式
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;           //设置 SPI 波特率预分频值为2，传输速度为 36Mhz/2
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                                 //在串行同步时钟的第二个边沿采样
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                                  //串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CRCPolynomial = 6;                                     //设置CRC校验多项式，提高通信可靠性，大于1即可
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                            //8位数据帧模式
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;           //双线双向全双工
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                           //设置数据传输为最高位在前
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                                //SPI主机模式
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                                    //通过软件控制NSS管理
	SPI_Init(USER_SPI_X , &SPI_InitStructure);
	
	//使能SPIx
	SPI_Cmd(USER_SPI_X , ENABLE);
	
}


/*
	*函数名： SPI_ReadWriteByte()
	*功  能： SPI发送接收字节函数
	*参  数： zdata：字节数据
	*返回值： 接收数据
	*作  者： 王峰
	*日  期： 2021.02.20
*/
u8 SPI_ReadWriteByte(u8 zdata)
{
	u8 times = 0;
	//等待发送区空
	while(SPI_I2S_GetFlagStatus(USER_SPI_X , SPI_I2S_FLAG_TXE) == RESET){
		times++;
		if(times > 200) break;
	}        
	SPI_I2S_SendData(USER_SPI_X , zdata);
	
	times = 0;
	//等待接收完一个 byte
	while(SPI_I2S_GetFlagStatus(USER_SPI_X , SPI_I2S_FLAG_RXNE) == RESET){
		times++;
		if(times > 200) break;
	}     
	return SPI_I2S_ReceiveData(USER_SPI_X);                                  //返回通过SPI2最近接收的数据
}


/*
	*函数名： SPI_SetSpeed()
	*功  能： SPI设置传输速度函数
	*参  数： SPI_BaudRatePrescaler：波特率预分频值
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.02.20
*/
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1 &= 0XFFC7;
	SPI2->CR1 |= SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(USER_SPI_X , ENABLE); 
}


