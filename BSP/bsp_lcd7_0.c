/*******************************************
	*文件名 ：  lcd7_0.c(7寸液晶屏驱动源文件)
	*作   者：  王峰
	*修改时间： 2021.01.05
	*版   本：  v1.0
*******************************************/




#include "bsp_lcd7_0.h"
#include "bsp_delay.h"


static u16 Current_TxtColor = BLACK;                    //默认设置文本颜色为黑色（前景色）
static u16 Current_BackColor = WHITE;                   //默认设置背景颜色为白色（背景色）



/*
	*函数名：Fsmc_Config()
	*功  能：FSMC配置函数
	*参  数： 无
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/
void Fsmc_Config(void)
{
  FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_ReadWriteTimingStructure;
	
	//使能FSMC时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC , ENABLE);
	
	//MD070SD FSMC读写时序结构体配置
	FSMC_ReadWriteTimingStructure.FSMC_AccessMode = FSMC_AccessMode_B;  //模式A
	FSMC_ReadWriteTimingStructure.FSMC_AddressSetupTime = 0x01;         //1+1个HCLK
	FSMC_ReadWriteTimingStructure.FSMC_DataSetupTime = 0x04;            //4是经验值,4+1个HCLK
	FSMC_ReadWriteTimingStructure.FSMC_AddressHoldTime = 0;             //sram没有用到
	FSMC_ReadWriteTimingStructure.FSMC_BusTurnAroundDuration = 0;       //sram没有用到
	FSMC_ReadWriteTimingStructure.FSMC_CLKDivision = 0;                 //sram没有用到
	FSMC_ReadWriteTimingStructure.FSMC_DataLatency = 0;                 //sram没有用到
	
	//MD070SD FSMC初始化结构体配置
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;          //读写同一个时序
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	
	//MD070SD FSMC没有用到
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	
	//时序结构体
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadWriteTimingStructure;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_ReadWriteTimingStructure;
	
	//将配置写入寄存器
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	
	//sram使能
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1 , ENABLE);
}


/*
	*函数名：MD070SD_Gpio_Config()
	*功  能：MD070SD引脚配置函数
	*参  数： 无
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/
void MD070SD_Gpio_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	//使能MD070SD FSMC相关引脚的时钟
	RCC_APB2PeriphClockCmd(FSMC_D0_GPIO_CLK | FSMC_D1_GPIO_CLK | FSMC_D2_GPIO_CLK |
	                       FSMC_D3_GPIO_CLK | FSMC_D4_GPIO_CLK | FSMC_D5_GPIO_CLK |
                         FSMC_D6_GPIO_CLK | FSMC_D7_GPIO_CLK | FSMC_D8_GPIO_CLK |	
	                       FSMC_D9_GPIO_CLK | FSMC_D10_GPIO_CLK | FSMC_D11_GPIO_CLK |
	                       FSMC_D12_GPIO_CLK | FSMC_D13_GPIO_CLK | FSMC_D14_GPIO_CLK |
	                       FSMC_D15_GPIO_CLK | MD070SD_CS_CLK | MD070SD_RD_CLK |
	                       MD070SD_WR_CLK | MD070SD_RS_CLK | MD070SD_RESET_CLK , ENABLE);
	
	//配置FSMC引脚工作模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//数据线D0-D15
	GPIO_InitStructure.GPIO_Pin = FSMC_D0_GPIO_PIN;
	GPIO_Init(FSMC_D0_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D1_GPIO_PIN;
	GPIO_Init(FSMC_D1_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D2_GPIO_PIN;
	GPIO_Init(FSMC_D2_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D3_GPIO_PIN;
	GPIO_Init(FSMC_D3_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D4_GPIO_PIN;
	GPIO_Init(FSMC_D4_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D5_GPIO_PIN;
	GPIO_Init(FSMC_D5_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D6_GPIO_PIN;
	GPIO_Init(FSMC_D6_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D7_GPIO_PIN;
	GPIO_Init(FSMC_D7_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D8_GPIO_PIN;
	GPIO_Init(FSMC_D8_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D9_GPIO_PIN;
	GPIO_Init(FSMC_D9_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D10_GPIO_PIN;
	GPIO_Init(FSMC_D10_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D11_GPIO_PIN;
	GPIO_Init(FSMC_D11_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D12_GPIO_PIN;
	GPIO_Init(FSMC_D12_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D13_GPIO_PIN;
	GPIO_Init(FSMC_D13_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D14_GPIO_PIN;
	GPIO_Init(FSMC_D14_GPIO_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D15_GPIO_PIN;
	GPIO_Init(FSMC_D15_GPIO_PORT , &GPIO_InitStructure);
	
	//片选引脚
	GPIO_InitStructure.GPIO_Pin = MD070SD_CS_PIN;
	GPIO_Init(MD070SD_CS_PORT , &GPIO_InitStructure);
	//读使能引脚
	GPIO_InitStructure.GPIO_Pin = MD070SD_RD_PIN;
	GPIO_Init(MD070SD_RD_PORT , &GPIO_InitStructure);
	//写使能引脚
	GPIO_InitStructure.GPIO_Pin = MD070SD_WR_PIN;
	GPIO_Init(MD070SD_WR_PORT , &GPIO_InitStructure);
	//数据/命令控制引脚
	GPIO_InitStructure.GPIO_Pin = MD070SD_RS_PIN;
	GPIO_Init(MD070SD_RS_PORT , &GPIO_InitStructure);
	
	//液晶屏复位引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = MD070SD_RESET_PIN;
	GPIO_Init(MD070SD_RESET_PORT , &GPIO_InitStructure);
    
}


/*
	*函数名：MD070SD_Init()
	*功  能：MD070SD初始化函数
	*参  数： 无
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/
void MD070SD_Init(void)
{
	MD070SD_Gpio_Config();
	Fsmc_Config();
	GPIO_ResetBits(MD070SD_RESET_PORT, MD070SD_RESET_PIN);
	Delay_Ms(20);
	GPIO_SetBits(MD070SD_RESET_PORT, MD070SD_RESET_PIN);
	Delay_Ms(20);
	MD070SD_Clear_Screen(0, 0, MD070SD_SCREEN_COL_WIDTH, MD070SD_SCREEN_ROW_WIDTH);
	MD070SD_Write_Cmd(BL_REG);
	MD070SD_Write_Data(16);
	MD070SD_Write_Cmd(ADDR_ADD_REG);
	MD070SD_Write_Data(0x0001);
}



/*
	*函数名：MD070SD_GetPixel()
	*功  能：MD070SD读取像素点数据函数
	*参  数： 像素点坐标：u16 posx , u16 posy
	*返回值： u16 ColorData
	*作  者： 王峰
	*日  期： 2021.01.19
*/
u16 MD070SD_GetPixel(u16 posx, u16 posy)
{
  u16 ColorData = 0xffff;
    
  //设置列起始坐标
	MD070SD_Write_Cmd(START_COL_REG);
	MD070SD_Write_Data(posx);
  //设置列终止坐标
  MD070SD_Write_Cmd(END_COL_REG);
	MD070SD_Write_Data(posx);
	
	//设置行起始坐标
	MD070SD_Write_Cmd(START_ROW_REG);
	MD070SD_Write_Data(posy);
  //设置行终止坐标
  MD070SD_Write_Cmd(END_ROW_REG);
	MD070SD_Write_Data(posy);

    
  //读取像素点颜色
	MD070SD_Write_Cmd(DATA_CH_REG);
  ColorData = MD070SD_Read_Data();
    
  return ColorData;
}


/*
	*函数名：MD070SD_PutPixel()
	*功  能：MD070SD填充像素点函数
	*参  数： 像素点坐标：u16 posx , u16 posy，RGB565颜色值：u16 color
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/
void MD070SD_PutPixel(u16 posx , u16 posy , u16 color)
{
  //设置列起始坐标
	MD070SD_Write_Cmd(START_COL_REG);
	MD070SD_Write_Data(posx);
  //设置列终止坐标
  MD070SD_Write_Cmd(END_COL_REG);
	MD070SD_Write_Data(posx);
	
	//设置行起始坐标
	MD070SD_Write_Cmd(START_ROW_REG);
	MD070SD_Write_Data(posy);
  //设置行终止坐标
  MD070SD_Write_Cmd(END_ROW_REG);
	MD070SD_Write_Data(posy);

    
  //设置像素点填充颜色
	MD070SD_Write_Cmd(DATA_CH_REG);
  MD070SD_Write_Data(color);
}



/*
	*函数名：MD070SD_Open_Window()
	*功  能：MD070SD开窗函数
	*参  数： 清屏起始点、终止点坐标：u16 start_x , u16 start_y , u16 width , u16 height
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/
void MD070SD_Open_Window(u16 start_x , u16 start_y , u16 end_x , u16 end_y)
{
	//设置列起始坐标
	MD070SD_Write_Cmd(START_COL_REG);
	MD070SD_Write_Data(start_x);
  //设置列终止坐标
  MD070SD_Write_Cmd(END_COL_REG);
	MD070SD_Write_Data(end_x);
	
	//设置行起始坐标
	MD070SD_Write_Cmd(START_ROW_REG);
	MD070SD_Write_Data(start_y);
  //设置行终止坐标
  MD070SD_Write_Cmd(END_ROW_REG);
	MD070SD_Write_Data(end_y);
}


/*
	*函数名：MD070SD_Draw_Rectangle()
	*功  能：MD070SD画矩形函数
	*参  数：起点终点坐标：u16 x0, u16 y0, u16 x, u16 y， RGB565颜色值：u16 color
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.19
*/
void MD070SD_Draw_Rectangle(u16 x0, u16 y0, u16 x, u16 y, u16 color)
{
	u32 i = 0;
	
	//对传入坐标进行校验和转换（保证起始点坐标小于终止点坐标）
	u16 width = (x>x0) ? (x-x0) : (x0-x);
	u16 height = (y>y0) ? (y-y0) : (y0-y);
	u16 sx = (x>x0) ? x0 : x;
	u16 sy = (y>y0) ? y0 : y;
	u16 ex = (x>x0) ? x : x0;
	u16 ey = (y>y0) ? y : y0;
	
	//开窗（矩形范围）
	MD070SD_Open_Window(sx , sy , ex , ey);

	//设置像素点填充颜色
	MD070SD_Write_Cmd(DATA_CH_REG);
	for(i = 0 ; i < (width * height) ; i++)
	{
			MD070SD_Write_Data(color);
	}  
}


/*
	*函数名：MD070SD_Draw_Circle()
	*功  能：MD070SD画圆函数
	*参  数： 圆参数结构体：CIRCLE circle，RGB565颜色值：u16 color，模式：u8 mode (0为空心，1为实心)
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/
void MD070SD_Draw_Circle(CIRCLE circle , u16 color , u8 mode)
{
	u16 start_x = 0 , start_y = 0 , height  = 0 , width = 0 ;
  start_x = circle.center_x - circle.radius ;
	start_y = circle.center_y - circle.radius ;
	height = 2 * circle.radius;
	width = 2 * circle.radius;
	
	
	if(mode ==  1){  
		u16 i = 0 , j = 0 ;
		u16 tempx = 0 , tempy = 0;
        
		//实心圆(填充画圆法)
		for(i = start_y ; i < (start_y + height) ; i++){
			if(i <= circle.center_y){
				tempy = circle.center_y - i;
			}
			else{
				tempy = i - circle.center_y; 
			}
			for(j = start_x ; j < (start_x + width) ; j++){
				if(j <= circle.center_x){
					tempx = circle.center_x - j;
				}
				else{
					tempx = j - circle.center_x; 
				}
				if((tempx*tempx + tempy*tempy) <= circle.radius * circle.radius){
					MD070SD_PutPixel(j , i , color);
				} 
			}
		} 
	}
	else{
		u16 offset_x = circle.center_x , offset_y = circle.center_y , x = 0 , y = circle.radius ;
		double d = 1.25 - circle.radius  ;
        
		//中点画圆法
		while(x < y){
			//根据圆的八分对称性
			MD070SD_PutPixel(x + offset_x , y + offset_y , color);
			MD070SD_PutPixel(x + offset_x , -y + offset_y , color);
			MD070SD_PutPixel(y + offset_x , x + offset_y , color);
			MD070SD_PutPixel(y + offset_x , -x + offset_y , color);
			MD070SD_PutPixel(-y + offset_x , x + offset_y , color);
			MD070SD_PutPixel(-y + offset_x , -x + offset_y , color);
			MD070SD_PutPixel(-x + offset_x , y + offset_y , color);
			MD070SD_PutPixel(-x + offset_x , -y + offset_y , color);
			
			if(d < 0){
				d = d + 2*x +3;
			}
			else{
				d = d +2*(x - y) +5;
				y--;			
			}
			x++;
		}
	}
}


/*
	*函数名：MD070SD_Draw_Line()
	*功  能：MD070SD画直线函数
	*参  数： 起点、终点坐标：u16 start_x , u16 start_y , u16 end_x , u16 end_y ，RGB565颜色值：u16 color
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/
void MD070SD_Draw_Line(u16 start_x , u16 start_y , u16 end_x , u16 end_y , u16 color)
{
  int16_t k = 0 , dx = 0 , dy = 0;
	u16 start_x0 = start_x , start_y0 = start_y;
	
	//确定增量方向，并且增量归一化
	if((end_x - start_x) & 0x8000){
		dx = -1;
	}
	else{
		dx = 1;
	}
	if((end_y - start_y) & 0x8000){
		dy = -1;
	}
	else{
		dy = 1;
	}
	
	//竖直直线
	if(start_x == end_x){
		while(start_y != end_y){
			MD070SD_PutPixel(start_x , start_y , color);
			start_y += dy;
		}
	}
	//水平直线
	else if(start_y == end_y){
		while(start_x != end_x){
			MD070SD_PutPixel(start_x , start_y , color);
			start_x += dx;
		}
	}
	//非特殊直线
	else{
		k = (end_y - start_y)/(end_x - start_x);
		while(start_x != end_x){
			MD070SD_PutPixel(start_x , start_y , color);
			start_x += dx;
			start_y = start_y0 + k*(start_x - start_x0);
		}
	}
}


/*
	*函数名：MD070SD_Draw_Cross()
	*功  能：MD070SD画十字架函数
	*参  数： 十字架中心坐标：u16 posx , u16 posy
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/    
void MD070SD_Draw_Cross(u16 posx , u16 posy)
{
  u16 level_sx = 0 , level_sy = 0 , level_ex = 0 , level_ey = 0;
	u16 vertical_sx = 0 , vertical_sy  = 0 , vertical_ex = 0 ,vertical_ey = 0;
	
	//计算十字架水平线起点，终点坐标
	level_sx = posx - 10;
	level_sy = posy;
	level_ex = posx + 10;
	level_ey = posy;
	
	//计算十字架竖直线起点，终点坐标
	vertical_sx = posx;
	vertical_sy = posy -10;
	vertical_ex = posx;
	vertical_ey = posy +10;
	
	//画出水平线和竖直线
	MD070SD_Draw_Line(level_sx , level_sy , level_ex , level_ey , Current_TxtColor);
	MD070SD_Draw_Line(vertical_sx , vertical_sy , vertical_ex , vertical_ey , Current_TxtColor);
}

    
 
/*
	*函数名：MD070SD_ShowChar_En()
	*功  能：MD070SD显示英文字符函数
	*参  数： 起始坐标和字符：u16 pos_x , u16 pos_y , u8 zdata，FONT font：字体
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/    
void MD070SD_ShowChar_En(u16 pos_x , u16 pos_y , u8 zdata, FONT font)
{
  u8 databuffer[64] = {0};
	u32 ASCII_offset_addr = 0;
	u8 i = 0 , j = 0, k = 0;
  u8 column = 0, rownum = 0;
	
	ASCII_offset_addr = ASCII_Addr_Tans(zdata, font);

	switch (font)
	{
		case ASCII_8X16: rownum = 16; column = 1; break;

		case ASCII_12X24: rownum = 24; column = 2; break;

		case ASCII_16X32: rownum = 32; column = 2; break;

		default:
			break;
	}
    
  W25QXX_Read(databuffer , ASCII_offset_addr , column*rownum);
	
	for (i = 0; i < rownum; i++) 
	{
		for (j = 0; j < column; j++) 
		{
			for (k = 0; k < 8; k++) 
			{
				if ((databuffer[i * column + j] << k) & 0x80) 
				{
					MD070SD_PutPixel(pos_x + j * 8 + k, pos_y + i, Current_TxtColor);
				}
			}
		}
	}
}
    

/*
	*函数名：MD070SD_ShowString_En()
	*功  能：MD070SD显示英文字符串函数
	*参  数： 起始坐标和字符串：u16 pos_x , u16 pos_y , u8 *zdata，FONT font：字体
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/    
void MD070SD_ShowString_En(u16 pos_x , u16 pos_y , u8 *zdata, FONT font)
{
  u16 i = 0;
  u8 column = 0, rownum = 0;
	u16 current_x = pos_x ,current_y = pos_y;

	switch (font)
	{
		case ASCII_8X16: rownum = 16; column = 8; break;

		case ASCII_12X24: rownum = 24; column = 12; break;

		case ASCII_16X32: rownum = 32; column = 16; break;

		default:
			break;
	}
	
	while(zdata[i] != '\0' )
	{
		
		MD070SD_ShowChar_En(current_x , current_y ,zdata[i++], font);     //显示当前字符
		current_x   +=  column;
		
		if(current_x >= MD070SD_SCREEN_COL_WIDTH)                         //如果到达x方向末尾，就要换行
		{                        
			current_x = 0;
			current_y += rownum;
		}
		
		if(current_y >= MD070SD_SCREEN_ROW_WIDTH)                         //如果到达y方向末尾，则结束，无法显示
		{                        
			break;
		}
	}
}
    
 
/*
	*函数名：MD070SD_ShowChar_Ch()
	*功  能：MD070SD显示汉字函数
	*参  数： 起始坐标和汉字：u16 pos_x , u16 pos_y , u8 *zdata，FONT font：字体
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/    
void MD070SD_ShowChar_Ch(u16 pos_x , u16 pos_y , u8 *zdata, FONT font)
{
  u8 databuffer[128] = {0};
	u32 GB2312_offset_addr = 0 ;
	u8 i = 0 , j = 0 , k = 0;
  u8 column = 0, rownum = 0;
	
	//将机内码转化成区位码再转成地址偏移
	GB2312_offset_addr = GB2312_Addr_Tans(zdata, font);
	
	switch((u8)font)
	{
		
		case GB2312_16x16: rownum = 16; column = 2; break;
		
		case GB2312_24x24: rownum = 24; column = 3; break;
		
		case GB2312_32x32: rownum = 32; column = 4; break;
	}
    
  W25QXX_Read(databuffer , GB2312_offset_addr , rownum*column);
	
	for(i = 0 ; i < rownum ; i++)
	{
		for(j = 0 ; j < column ; j++)
		{
			for(k = 0 ; k < 8 ; k++)
			{
				if((databuffer[i*column + j] << k) & 0x80)
				{
					MD070SD_PutPixel(pos_x + j*8 + k ,pos_y + i, Current_TxtColor);
				}
			}
		}
	}
}
    
    
/*
	*函数名：MD070SD_ShowString_Ch()
	*功  能：MD070SD显示汉字字符串函数
	*参  数： 起始坐标和字符串：u16 pos_x , u16 pos_y , u8 *zdata，FONT font：字体
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/   
void MD070SD_ShowString_Ch(u16 pos_x , u16 pos_y , u8 *zdata, FONT font)
{
  u16 i = 0;
  u8 column = 0, rownum = 0;
	u8 chinese_char[2] = {0};
	u16 current_x = pos_x ,current_y = pos_y;                               //位置指针                        
	
	switch((u8)font)
	{
		case GB2312_16x16: column = 16; rownum = 16; break;
		
		case GB2312_24x24: column = 24; rownum = 24; break;
		
		case GB2312_32x32: column = 32; rownum = 32; break;
    default: break;
		
	}
	
	while(zdata[i] != '\0'){
		
		chinese_char[0] = zdata[i++];                                      //将汉字信息放到构建的汉字模拟变量（数组）
		chinese_char[1] = zdata[i++];
		
		MD070SD_ShowChar_Ch(current_x , current_y , chinese_char, font);   //显示当前汉字字符
		current_x += column;
		
		if(current_x >= MD070SD_SCREEN_COL_WIDTH)                          //如果到达x方向末尾，就要换行
		{                        
			current_x = 0;
			current_y += rownum;
		}
		
		if(current_y >= MD070SD_SCREEN_ROW_WIDTH)                          //如果到达y方向末尾，则结束，无法显示
		{                         
			break;
		}
	}
} 
    
    
/*
	*函数名：MD070SD_Clear_Screen()
	*功  能：MD070SD清屏函数
	*参  数： 清屏起始点坐标、宽度和高度：u16 start_x , u16 start_y , u16 width , u16 height
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/   
void MD070SD_Clear_Screen(u16 start_x , u16 start_y , u16 width , u16 height)
{
  u32 i = 0 ; 
	
	//全屏开窗
	MD070SD_Open_Window(start_x , start_y , width , height);
	
	//设置像素点填充颜色为背景色
	MD070SD_Write_Cmd(DATA_CH_REG);
	for(i = 0 ; i < (width * height) ; i++){
		MD070SD_Write_Data(Current_BackColor);
	}
}
    

/*
	*函数名：MD070SD_Set_Txtcolor()
	*功  能：MD070SD设置文本色函数
	*参  数： u16 color：RGB565颜色值
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/    
void MD070SD_Set_Txtcolor(u16 color)
{
  Current_TxtColor = color;
}
    

/*
	*函数名：MD070SD_Set_Backcolor()
	*功  能：MD070SD设置背景色函数
	*参  数：u16 color：RGB565颜色值
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/    
void MD070SD_Set_Backcolor(u16 color)
{
  Current_BackColor = color;
  MD070SD_Clear_Screen(0, 0, MD070SD_SCREEN_ROW_WIDTH, MD070SD_SCREEN_COL_WIDTH);
}
    

/*
	*函数名：MD070SD_Set_Backcolor()
	*功  能：MD070SD设置背景色函数
	*参  数：u16 color：RGB565颜色值
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.01.05
*/    
void MD070SD_Set_BackLight(u8 BL_Value)
{
  MD070SD_Write_Cmd(BL_REG);
  MD070SD_Write_Data(BL_Value);
}
    


