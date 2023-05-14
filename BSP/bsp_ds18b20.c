#include "bsp_ds18b20.h"
#include "bsp_delay.h"
#include "stdio.h"

//u8 DS18B20_ID[8] = {0x28, 0xff, 0x64, 0x6, 0xf9, 0x56, 0xb7, 0x20};
u8 DS18B20_ID[8] = {0x28, 0xff, 0x64, 0x6, 0xf9, 0x2d, 0x17, 0x1a};

/* 
	*函数名：Ds18b20_GPIO_Config()
	*功  能：DS18B20引脚配置函数
	*作  者：WF
	*参  数：无
	*返回值：无
	*时  间：2021.04.01
*/
void Ds18b20_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
	DS18B20_DQ_GPIO_APBxClkCmd(DS18B20_DQ_GPIO_CLK, ENABLE);	  //使能DS18B20_DQ端口时钟
	
	/* DS18B20_DQ端口配置 */
	GPIO_InitStructure.GPIO_Pin = DS18B20_DQ_GPIO_PIN;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	          
	GPIO_Init(DS18B20_DQ_GPIO_PORT, &GPIO_InitStructure);			  
	
	GPIO_SetBits(DS18B20_DQ_GPIO_PORT, DS18B20_DQ_GPIO_PIN);
}


/* 
	*函数名：Ds18b20_Reset()
	*功  能：DS18B20复位函数
	*作  者：WF
	*参  数：无
	*返回值：返回1：复位成功， 返回0：复位失败
	*时  间：2021.04.01
*/
u8 Ds18b20_Reset(void)
{
	DS18B20_DQ_OUT();
	DS18B20_DQ_RESET();           //主机拉低总线480us
	Delay_Us(480);     
	DS18B20_DQ_SET();             //主机释放总线
	Delay_Us(60);                 //DS18B20等待60us做好准备

	DS18B20_DQ_IN();
	if(DS18B20_DQ_READ() == 0)    //DS18B20将总线拉低则复位成功
	{
		Delay_Us(480);
		return 1;
	}
	
	return 0;                     //复位失败
}



/* 
	*函数名：Ds18b20_Init()
	*功  能：DS18B20初始化函数
	*作  者：WF
	*参  数：无
	*返回值：返回1：复位成功， 返回0：复位失败
	*时  间：2021.04.01
*/
u8 Ds18b20_Init(void)
{
	u8 flag = 0;
	u8 times = 0;
	
	Ds18b20_GPIO_Config();
	
//  printf("1\r\n");
	
	while(flag == 0 && times<1) //超时检测
	{
		flag  = Ds18b20_Reset();
		//Delay_Ms(1);
		//printf("times = %d\r\n", times);
		times++;
	}
//	printf("%d \r\n",flag);
	//printf("2\r\n");
	
	if(times >= 1)
	{
		return 0;   //超时，初始化失败
	}
	else 
	{
		return 1;   //初始化成功
	}
}


/* 
	*函数名：Ds18b20_WriteByte()
	*功  能：DS18B20写字节函数函数
	*作  者：WF
	*参  数：u8 zdata：待写入字节数据
	*返回值：无
	*时  间：2021.04.01
*/
void Ds18b20_WriteByte(u8 zdata)
{
	u8 i = 0;
	
	DS18B20_DQ_OUT();
	for(i =  0; i < 8; i++)
	{
		DS18B20_DQ_RESET();
		if(zdata&0x01)
		{
			DS18B20_DQ_SET();      //将数据‘1’送入总线，等待DS18B20开始采样
		}
		else
		{
			DS18B20_DQ_RESET();    //将数据‘0’送入总线，等待DS18B20开始采样
		}
		Delay_Us(60);        
		DS18B20_DQ_SET();        //大于60us后完成一位写时隙，主机释放总线
		Delay_Us(2);
		zdata >>= 1;
	}
}


/* 
	*函数名：Ds18b20_ReadByte()
	*功  能：DS18B20读字节函数
	*作  者：WF
	*参  数：无
	*返回值：读取的字节数据
	*时  间：2021.04.01
*/
u8 Ds18b20_ReadByte(void)
{
	u8 i = 0;
	u8 tempdata = 0;
	
	for(i = 0; i < 8; i++)
	{
		DS18B20_DQ_OUT();
		tempdata >>= 1;
		DS18B20_DQ_RESET();          //主机拉低总线
		Delay_Us(2);
		DS18B20_DQ_SET();            //大于1us后，主机释放总线
		
		DS18B20_DQ_IN();
		if(DS18B20_DQ_READ() == 1)   //主机在15us内采样DS18B20送到总线的数据
		{
				 tempdata |= 0x80;
		}
		Delay_Us(60);
	}
	return tempdata;
}


/* 
	*函数名：Ds18b20_ReadSingleTemp()
	*功  能：DS18B20读取单点温度函数
	*作  者：WF
	*参  数：u8 Num：DS18B20编号
	*返回值：读取的温度数据
	*时  间：2021.04.01
*/
u16 Ds18b20_ReadSingleTemp(u8 Num)
{
	u8 LByte = 0, HByte = 0;
	u16 temperature = 0;

	Ds18b20_Reset();
	Ds18b20_MatchRom(DS18B20_ID);                 //匹配ROM
//	Ds18b20_WriteByte(DS18B20_SKIP_ROM);
	Ds18b20_WriteByte(DS18B20_CONVERT);           //开启温度转换
	Delay_Ms(5);                                  //延时大小和分辨率有关，操作系统下延时可以将任务挂起
	
	Ds18b20_Reset();
	Ds18b20_MatchRom(DS18B20_ID);                 //匹配ROM
//	Ds18b20_WriteByte(DS18B20_SKIP_ROM);	
	Ds18b20_WriteByte(DS18B20_READ_REG);          //发送读暂温度指令
	
	LByte = Ds18b20_ReadByte();                   //读取温度低8位
	HByte = Ds18b20_ReadByte();                   //读取温度高8位

	
	if((HByte&0xf8) == 0xf8)                      //如果是负温度值需要将补码转化成原码
	{
		temperature = (HByte<<8) + LByte;
		temperature -= 1;
		temperature = ~temperature;
		temperature = temperature*0.0625 * 10;
	}
	else                                          //如果是正温度
	{
		temperature = ((HByte&0x07)*256 + LByte) * 0.0625 * 10; //保留一位小数
	}
	
	return temperature;
}


/* 
	*函数名：Ds18b20_ReadMultiTemp()
	*功  能：DS18B20读取多点温度函数
	*作  者：WF
	*参  数：u16* TmpBuffer：温度数据缓存Buffer
	*返回值：无
	*时  间：2021.04.08
*/
void Ds18b20_ReadMultiTemp(u16* TempBuffer)
{
	u8 LByte = 0, HByte = 0;
	u16 temperature = 0;
	u8 i = 0;
	
	Ds18b20_Reset();
	Ds18b20_WriteByte(DS18B20_SKIP_ROM);          //跳过ROM
	Ds18b20_WriteByte(DS18B20_CONVERT);           //统一开启温度转换
	Delay_Ms(1000);             
	
	/* 循环读取每个DS18B20的温度 */
	for(i = 0; i < DS18B20_NUM; i++)
	{
		Ds18b20_Reset();
		//Ds18b20_MatchRom(BAT_Module_DS18B20_ID[i]);   //匹配ROM
		Ds18b20_WriteByte(DS18B20_READ_REG);          //发送读暂温度指令
		
		LByte = Ds18b20_ReadByte(); //读取温度低8位
		HByte = Ds18b20_ReadByte(); //读取温度高8位
		
		if((HByte&0xf8) == 0xf8)              //如果是负温度值需要将补码转化成原码
		{
			temperature = (HByte<<8) + LByte;
			temperature -= 1;
			temperature = ~temperature;
			temperature = temperature*0.0625;
		}
		else  //如果是正温度
		{
			temperature = ((HByte&0x07)*256 + LByte) * 0.0625 * 10; //保留一位小数
		}
		
		TempBuffer[i] = temperature;
	}
}


/* 
	*函数名：Ds18b20_ReadRom()
	*功  能：DS18B20读取Rom函数
	*作  者：WF
	*参  数：u8 *zdata：保存64位ROM序列的数组
	*返回值：无
	*时  间：2021.04.01
*/
void Ds18b20_ReadRom(u8 *zdata)
{
	u8 i = 0;
	Ds18b20_Reset();
	Ds18b20_WriteByte(DS18B20_READ_ROM); //发送读ROM取指令

	/* 从机发送64位ROM序列给主机读取 */
	for(i = 0; i < 8; i++)
	{
		zdata[i] = Ds18b20_ReadByte();
	}
}


/* 
	*函数名：Ds18b20_MatchRom()
	*功  能：DS18B20匹配ROM函数
	*作  者：WF
	*参  数：u8 *zdata：待匹配的64位ROM序列数组
	*返回值：无
	*时  间：2021.04.01
*/
void Ds18b20_MatchRom(u8 *zdata)
{
	u8 i = 0;
	
	Ds18b20_WriteByte(DS18B20_MATCH_ROM); //发送匹配ROM指令
	
	/* 主机发送64位ROM序列到从机进行匹配 */
	for(i = 0; i < 8; i++)
	{
		Ds18b20_WriteByte(zdata[i]);
	}
}


