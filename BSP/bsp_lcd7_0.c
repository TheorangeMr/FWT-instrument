/*******************************************
	*�ļ��� ��  lcd7_0.c(7��Һ��������Դ�ļ�)
	*��   �ߣ�  ����
	*�޸�ʱ�䣺 2021.01.05
	*��   ����  v1.0
*******************************************/




#include "bsp_lcd7_0.h"
#include "bsp_delay.h"


static u16 Current_TxtColor = BLACK;                    //Ĭ�������ı���ɫΪ��ɫ��ǰ��ɫ��
static u16 Current_BackColor = WHITE;                   //Ĭ�����ñ�����ɫΪ��ɫ������ɫ��



/*
	*��������Fsmc_Config()
	*��  �ܣ�FSMC���ú���
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
*/
void Fsmc_Config(void)
{
  FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_ReadWriteTimingStructure;
	
	//ʹ��FSMCʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC , ENABLE);
	
	//MD070SD FSMC��дʱ��ṹ������
	FSMC_ReadWriteTimingStructure.FSMC_AccessMode = FSMC_AccessMode_B;  //ģʽA
	FSMC_ReadWriteTimingStructure.FSMC_AddressSetupTime = 0x01;         //1+1��HCLK
	FSMC_ReadWriteTimingStructure.FSMC_DataSetupTime = 0x04;            //4�Ǿ���ֵ,4+1��HCLK
	FSMC_ReadWriteTimingStructure.FSMC_AddressHoldTime = 0;             //sramû���õ�
	FSMC_ReadWriteTimingStructure.FSMC_BusTurnAroundDuration = 0;       //sramû���õ�
	FSMC_ReadWriteTimingStructure.FSMC_CLKDivision = 0;                 //sramû���õ�
	FSMC_ReadWriteTimingStructure.FSMC_DataLatency = 0;                 //sramû���õ�
	
	//MD070SD FSMC��ʼ���ṹ������
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;          //��дͬһ��ʱ��
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	
	//MD070SD FSMCû���õ�
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	
	//ʱ��ṹ��
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadWriteTimingStructure;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_ReadWriteTimingStructure;
	
	//������д��Ĵ���
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	
	//sramʹ��
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1 , ENABLE);
}


/*
	*��������MD070SD_Gpio_Config()
	*��  �ܣ�MD070SD�������ú���
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
*/
void MD070SD_Gpio_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	//ʹ��MD070SD FSMC������ŵ�ʱ��
	RCC_APB2PeriphClockCmd(FSMC_D0_GPIO_CLK | FSMC_D1_GPIO_CLK | FSMC_D2_GPIO_CLK |
	                       FSMC_D3_GPIO_CLK | FSMC_D4_GPIO_CLK | FSMC_D5_GPIO_CLK |
                         FSMC_D6_GPIO_CLK | FSMC_D7_GPIO_CLK | FSMC_D8_GPIO_CLK |	
	                       FSMC_D9_GPIO_CLK | FSMC_D10_GPIO_CLK | FSMC_D11_GPIO_CLK |
	                       FSMC_D12_GPIO_CLK | FSMC_D13_GPIO_CLK | FSMC_D14_GPIO_CLK |
	                       FSMC_D15_GPIO_CLK | MD070SD_CS_CLK | MD070SD_RD_CLK |
	                       MD070SD_WR_CLK | MD070SD_RS_CLK | MD070SD_RESET_CLK , ENABLE);
	
	//����FSMC���Ź���ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//������D0-D15
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
	
	//Ƭѡ����
	GPIO_InitStructure.GPIO_Pin = MD070SD_CS_PIN;
	GPIO_Init(MD070SD_CS_PORT , &GPIO_InitStructure);
	//��ʹ������
	GPIO_InitStructure.GPIO_Pin = MD070SD_RD_PIN;
	GPIO_Init(MD070SD_RD_PORT , &GPIO_InitStructure);
	//дʹ������
	GPIO_InitStructure.GPIO_Pin = MD070SD_WR_PIN;
	GPIO_Init(MD070SD_WR_PORT , &GPIO_InitStructure);
	//����/�����������
	GPIO_InitStructure.GPIO_Pin = MD070SD_RS_PIN;
	GPIO_Init(MD070SD_RS_PORT , &GPIO_InitStructure);
	
	//Һ������λ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = MD070SD_RESET_PIN;
	GPIO_Init(MD070SD_RESET_PORT , &GPIO_InitStructure);
    
}


/*
	*��������MD070SD_Init()
	*��  �ܣ�MD070SD��ʼ������
	*��  ���� ��
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
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
	*��������MD070SD_GetPixel()
	*��  �ܣ�MD070SD��ȡ���ص����ݺ���
	*��  ���� ���ص����꣺u16 posx , u16 posy
	*����ֵ�� u16 ColorData
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.19
*/
u16 MD070SD_GetPixel(u16 posx, u16 posy)
{
  u16 ColorData = 0xffff;
    
  //��������ʼ����
	MD070SD_Write_Cmd(START_COL_REG);
	MD070SD_Write_Data(posx);
  //��������ֹ����
  MD070SD_Write_Cmd(END_COL_REG);
	MD070SD_Write_Data(posx);
	
	//��������ʼ����
	MD070SD_Write_Cmd(START_ROW_REG);
	MD070SD_Write_Data(posy);
  //��������ֹ����
  MD070SD_Write_Cmd(END_ROW_REG);
	MD070SD_Write_Data(posy);

    
  //��ȡ���ص���ɫ
	MD070SD_Write_Cmd(DATA_CH_REG);
  ColorData = MD070SD_Read_Data();
    
  return ColorData;
}


/*
	*��������MD070SD_PutPixel()
	*��  �ܣ�MD070SD������ص㺯��
	*��  ���� ���ص����꣺u16 posx , u16 posy��RGB565��ɫֵ��u16 color
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
*/
void MD070SD_PutPixel(u16 posx , u16 posy , u16 color)
{
  //��������ʼ����
	MD070SD_Write_Cmd(START_COL_REG);
	MD070SD_Write_Data(posx);
  //��������ֹ����
  MD070SD_Write_Cmd(END_COL_REG);
	MD070SD_Write_Data(posx);
	
	//��������ʼ����
	MD070SD_Write_Cmd(START_ROW_REG);
	MD070SD_Write_Data(posy);
  //��������ֹ����
  MD070SD_Write_Cmd(END_ROW_REG);
	MD070SD_Write_Data(posy);

    
  //�������ص������ɫ
	MD070SD_Write_Cmd(DATA_CH_REG);
  MD070SD_Write_Data(color);
}



/*
	*��������MD070SD_Open_Window()
	*��  �ܣ�MD070SD��������
	*��  ���� ������ʼ�㡢��ֹ�����꣺u16 start_x , u16 start_y , u16 width , u16 height
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
*/
void MD070SD_Open_Window(u16 start_x , u16 start_y , u16 end_x , u16 end_y)
{
	//��������ʼ����
	MD070SD_Write_Cmd(START_COL_REG);
	MD070SD_Write_Data(start_x);
  //��������ֹ����
  MD070SD_Write_Cmd(END_COL_REG);
	MD070SD_Write_Data(end_x);
	
	//��������ʼ����
	MD070SD_Write_Cmd(START_ROW_REG);
	MD070SD_Write_Data(start_y);
  //��������ֹ����
  MD070SD_Write_Cmd(END_ROW_REG);
	MD070SD_Write_Data(end_y);
}


/*
	*��������MD070SD_Draw_Rectangle()
	*��  �ܣ�MD070SD�����κ���
	*��  ��������յ����꣺u16 x0, u16 y0, u16 x, u16 y�� RGB565��ɫֵ��u16 color
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.19
*/
void MD070SD_Draw_Rectangle(u16 x0, u16 y0, u16 x, u16 y, u16 color)
{
	u32 i = 0;
	
	//�Դ����������У���ת������֤��ʼ������С����ֹ�����꣩
	u16 width = (x>x0) ? (x-x0) : (x0-x);
	u16 height = (y>y0) ? (y-y0) : (y0-y);
	u16 sx = (x>x0) ? x0 : x;
	u16 sy = (y>y0) ? y0 : y;
	u16 ex = (x>x0) ? x : x0;
	u16 ey = (y>y0) ? y : y0;
	
	//���������η�Χ��
	MD070SD_Open_Window(sx , sy , ex , ey);

	//�������ص������ɫ
	MD070SD_Write_Cmd(DATA_CH_REG);
	for(i = 0 ; i < (width * height) ; i++)
	{
			MD070SD_Write_Data(color);
	}  
}


/*
	*��������MD070SD_Draw_Circle()
	*��  �ܣ�MD070SD��Բ����
	*��  ���� Բ�����ṹ�壺CIRCLE circle��RGB565��ɫֵ��u16 color��ģʽ��u8 mode (0Ϊ���ģ�1Ϊʵ��)
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
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
        
		//ʵ��Բ(��仭Բ��)
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
        
		//�е㻭Բ��
		while(x < y){
			//����Բ�İ˷ֶԳ���
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
	*��������MD070SD_Draw_Line()
	*��  �ܣ�MD070SD��ֱ�ߺ���
	*��  ���� ��㡢�յ����꣺u16 start_x , u16 start_y , u16 end_x , u16 end_y ��RGB565��ɫֵ��u16 color
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
*/
void MD070SD_Draw_Line(u16 start_x , u16 start_y , u16 end_x , u16 end_y , u16 color)
{
  int16_t k = 0 , dx = 0 , dy = 0;
	u16 start_x0 = start_x , start_y0 = start_y;
	
	//ȷ���������򣬲���������һ��
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
	
	//��ֱֱ��
	if(start_x == end_x){
		while(start_y != end_y){
			MD070SD_PutPixel(start_x , start_y , color);
			start_y += dy;
		}
	}
	//ˮƽֱ��
	else if(start_y == end_y){
		while(start_x != end_x){
			MD070SD_PutPixel(start_x , start_y , color);
			start_x += dx;
		}
	}
	//������ֱ��
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
	*��������MD070SD_Draw_Cross()
	*��  �ܣ�MD070SD��ʮ�ּܺ���
	*��  ���� ʮ�ּ��������꣺u16 posx , u16 posy
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
*/    
void MD070SD_Draw_Cross(u16 posx , u16 posy)
{
  u16 level_sx = 0 , level_sy = 0 , level_ex = 0 , level_ey = 0;
	u16 vertical_sx = 0 , vertical_sy  = 0 , vertical_ex = 0 ,vertical_ey = 0;
	
	//����ʮ�ּ�ˮƽ����㣬�յ�����
	level_sx = posx - 10;
	level_sy = posy;
	level_ex = posx + 10;
	level_ey = posy;
	
	//����ʮ�ּ���ֱ����㣬�յ�����
	vertical_sx = posx;
	vertical_sy = posy -10;
	vertical_ex = posx;
	vertical_ey = posy +10;
	
	//����ˮƽ�ߺ���ֱ��
	MD070SD_Draw_Line(level_sx , level_sy , level_ex , level_ey , Current_TxtColor);
	MD070SD_Draw_Line(vertical_sx , vertical_sy , vertical_ex , vertical_ey , Current_TxtColor);
}

    
 
/*
	*��������MD070SD_ShowChar_En()
	*��  �ܣ�MD070SD��ʾӢ���ַ�����
	*��  ���� ��ʼ������ַ���u16 pos_x , u16 pos_y , u8 zdata��FONT font������
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
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
	*��������MD070SD_ShowString_En()
	*��  �ܣ�MD070SD��ʾӢ���ַ�������
	*��  ���� ��ʼ������ַ�����u16 pos_x , u16 pos_y , u8 *zdata��FONT font������
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
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
		
		MD070SD_ShowChar_En(current_x , current_y ,zdata[i++], font);     //��ʾ��ǰ�ַ�
		current_x   +=  column;
		
		if(current_x >= MD070SD_SCREEN_COL_WIDTH)                         //�������x����ĩβ����Ҫ����
		{                        
			current_x = 0;
			current_y += rownum;
		}
		
		if(current_y >= MD070SD_SCREEN_ROW_WIDTH)                         //�������y����ĩβ����������޷���ʾ
		{                        
			break;
		}
	}
}
    
 
/*
	*��������MD070SD_ShowChar_Ch()
	*��  �ܣ�MD070SD��ʾ���ֺ���
	*��  ���� ��ʼ����ͺ��֣�u16 pos_x , u16 pos_y , u8 *zdata��FONT font������
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
*/    
void MD070SD_ShowChar_Ch(u16 pos_x , u16 pos_y , u8 *zdata, FONT font)
{
  u8 databuffer[128] = {0};
	u32 GB2312_offset_addr = 0 ;
	u8 i = 0 , j = 0 , k = 0;
  u8 column = 0, rownum = 0;
	
	//��������ת������λ����ת�ɵ�ַƫ��
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
	*��������MD070SD_ShowString_Ch()
	*��  �ܣ�MD070SD��ʾ�����ַ�������
	*��  ���� ��ʼ������ַ�����u16 pos_x , u16 pos_y , u8 *zdata��FONT font������
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
*/   
void MD070SD_ShowString_Ch(u16 pos_x , u16 pos_y , u8 *zdata, FONT font)
{
  u16 i = 0;
  u8 column = 0, rownum = 0;
	u8 chinese_char[2] = {0};
	u16 current_x = pos_x ,current_y = pos_y;                               //λ��ָ��                        
	
	switch((u8)font)
	{
		case GB2312_16x16: column = 16; rownum = 16; break;
		
		case GB2312_24x24: column = 24; rownum = 24; break;
		
		case GB2312_32x32: column = 32; rownum = 32; break;
    default: break;
		
	}
	
	while(zdata[i] != '\0'){
		
		chinese_char[0] = zdata[i++];                                      //��������Ϣ�ŵ������ĺ���ģ����������飩
		chinese_char[1] = zdata[i++];
		
		MD070SD_ShowChar_Ch(current_x , current_y , chinese_char, font);   //��ʾ��ǰ�����ַ�
		current_x += column;
		
		if(current_x >= MD070SD_SCREEN_COL_WIDTH)                          //�������x����ĩβ����Ҫ����
		{                        
			current_x = 0;
			current_y += rownum;
		}
		
		if(current_y >= MD070SD_SCREEN_ROW_WIDTH)                          //�������y����ĩβ����������޷���ʾ
		{                         
			break;
		}
	}
} 
    
    
/*
	*��������MD070SD_Clear_Screen()
	*��  �ܣ�MD070SD��������
	*��  ���� ������ʼ�����ꡢ��Ⱥ͸߶ȣ�u16 start_x , u16 start_y , u16 width , u16 height
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
*/   
void MD070SD_Clear_Screen(u16 start_x , u16 start_y , u16 width , u16 height)
{
  u32 i = 0 ; 
	
	//ȫ������
	MD070SD_Open_Window(start_x , start_y , width , height);
	
	//�������ص������ɫΪ����ɫ
	MD070SD_Write_Cmd(DATA_CH_REG);
	for(i = 0 ; i < (width * height) ; i++){
		MD070SD_Write_Data(Current_BackColor);
	}
}
    

/*
	*��������MD070SD_Set_Txtcolor()
	*��  �ܣ�MD070SD�����ı�ɫ����
	*��  ���� u16 color��RGB565��ɫֵ
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
*/    
void MD070SD_Set_Txtcolor(u16 color)
{
  Current_TxtColor = color;
}
    

/*
	*��������MD070SD_Set_Backcolor()
	*��  �ܣ�MD070SD���ñ���ɫ����
	*��  ����u16 color��RGB565��ɫֵ
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
*/    
void MD070SD_Set_Backcolor(u16 color)
{
  Current_BackColor = color;
  MD070SD_Clear_Screen(0, 0, MD070SD_SCREEN_ROW_WIDTH, MD070SD_SCREEN_COL_WIDTH);
}
    

/*
	*��������MD070SD_Set_Backcolor()
	*��  �ܣ�MD070SD���ñ���ɫ����
	*��  ����u16 color��RGB565��ɫֵ
	*����ֵ�� ��
	*��  �ߣ� ����
	*��  �ڣ� 2021.01.05
*/    
void MD070SD_Set_BackLight(u8 BL_Value)
{
  MD070SD_Write_Cmd(BL_REG);
  MD070SD_Write_Data(BL_Value);
}
    


