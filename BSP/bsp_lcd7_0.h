/*******************************************
	*�ļ��� ��  lcd7_0.h(7��Һ��������ͷ�ļ�)
	*��   �ߣ�  ����
	*�޸�ʱ�䣺 2021.01.05
	*��   ����  v1.0
*******************************************/



#ifndef _LCD7_0_H
#define _LCD7_0_H

#include "sys.h"
#include "bsp_w25qxx.h"


/*   
     Һ��������оƬMD070SD ���� 8080 ʱ�� 16bit �������߽ӿڣ�8080ʱ����������ģ�⣬
     Ҳ������FSMC����������������FSMC����8080ʱ���������
*/


/********************************FSMC������ź궨��***********************************/

/* FSMC������ */

#define FSMC_D0_GPIO_PORT        GPIOD
#define FSMC_D0_GPIO_CLK         RCC_APB2Periph_GPIOD
#define FSMC_D0_GPIO_PIN         GPIO_Pin_14

#define FSMC_D1_GPIO_PORT        GPIOD
#define FSMC_D1_GPIO_CLK         RCC_APB2Periph_GPIOD
#define FSMC_D1_GPIO_PIN         GPIO_Pin_15

#define FSMC_D2_GPIO_PORT        GPIOD
#define FSMC_D2_GPIO_CLK         RCC_APB2Periph_GPIOD
#define FSMC_D2_GPIO_PIN         GPIO_Pin_0

#define FSMC_D3_GPIO_PORT        GPIOD
#define FSMC_D3_GPIO_CLK         RCC_APB2Periph_GPIOD
#define FSMC_D3_GPIO_PIN         GPIO_Pin_1

#define FSMC_D4_GPIO_PORT        GPIOE
#define FSMC_D4_GPIO_CLK         RCC_APB2Periph_GPIOE
#define FSMC_D4_GPIO_PIN         GPIO_Pin_7

#define FSMC_D5_GPIO_PORT        GPIOE
#define FSMC_D5_GPIO_CLK         RCC_APB2Periph_GPIOE
#define FSMC_D5_GPIO_PIN         GPIO_Pin_8

#define FSMC_D6_GPIO_PORT        GPIOE
#define FSMC_D6_GPIO_CLK         RCC_APB2Periph_GPIOE
#define FSMC_D6_GPIO_PIN         GPIO_Pin_9

#define FSMC_D7_GPIO_PORT        GPIOE
#define FSMC_D7_GPIO_CLK         RCC_APB2Periph_GPIOE
#define FSMC_D7_GPIO_PIN         GPIO_Pin_10

#define FSMC_D8_GPIO_PORT        GPIOE
#define FSMC_D8_GPIO_CLK         RCC_APB2Periph_GPIOE
#define FSMC_D8_GPIO_PIN         GPIO_Pin_11

#define FSMC_D9_GPIO_PORT        GPIOE
#define FSMC_D9_GPIO_CLK         RCC_APB2Periph_GPIOE
#define FSMC_D9_GPIO_PIN         GPIO_Pin_12

#define FSMC_D10_GPIO_PORT       GPIOE
#define FSMC_D10_GPIO_CLK        RCC_APB2Periph_GPIOE
#define FSMC_D10_GPIO_PIN        GPIO_Pin_13

#define FSMC_D11_GPIO_PORT       GPIOE
#define FSMC_D11_GPIO_CLK        RCC_APB2Periph_GPIOE
#define FSMC_D11_GPIO_PIN        GPIO_Pin_14

#define FSMC_D12_GPIO_PORT       GPIOE
#define FSMC_D12_GPIO_CLK        RCC_APB2Periph_GPIOE
#define FSMC_D12_GPIO_PIN        GPIO_Pin_15

#define FSMC_D13_GPIO_PORT       GPIOD
#define FSMC_D13_GPIO_CLK        RCC_APB2Periph_GPIOD
#define FSMC_D13_GPIO_PIN        GPIO_Pin_8

#define FSMC_D14_GPIO_PORT       GPIOD
#define FSMC_D14_GPIO_CLK        RCC_APB2Periph_GPIOD
#define FSMC_D14_GPIO_PIN        GPIO_Pin_9

#define FSMC_D15_GPIO_PORT       GPIOD
#define FSMC_D15_GPIO_CLK        RCC_APB2Periph_GPIOD
#define FSMC_D15_GPIO_PIN        GPIO_Pin_10

/* FSMC�����ź��� */

//Ƭѡ���ţ�����FSMC_NE1
#define      MD070SD_CS_CLK           RCC_APB2Periph_GPIOD
#define      MD070SD_CS_PORT          GPIOD
#define      MD070SD_CS_PIN           GPIO_Pin_7
//��ʹ�����ţ�����FSMC_NOE
#define      MD070SD_RD_CLK           RCC_APB2Periph_GPIOD
#define      MD070SD_RD_PORT          GPIOD
#define      MD070SD_RD_PIN           GPIO_Pin_4
//дʹ�����ţ�����FSMC_NWE
#define      MD070SD_WR_CLK           RCC_APB2Periph_GPIOD
#define      MD070SD_WR_PORT          GPIOD
#define      MD070SD_WR_PIN           GPIO_Pin_5
//����/����ѡ�����ţ�����FSMC_A16
#define      MD070SD_RS_CLK           RCC_APB2Periph_GPIOD
#define      MD070SD_RS_PORT          GPIOD
#define      MD070SD_RS_PIN           GPIO_Pin_11
//Һ������λ����
#define      MD070SD_RESET_CLK        RCC_APB2Periph_GPIOA
#define      MD070SD_RESET_PORT       GPIOA
#define      MD070SD_RESET_PIN        GPIO_Pin_1


//д���ݵ�ַ��������D/CXҲΪ�ߣ�
#define      MD070SD_DATA_ADDR        (vu16 *)(0x60000000 | (1<<(16+1)))
//д�����ַ
#define      MD070SD_CMD_ADDR         (vu16 *)(0x60000000 & ~(1<<(16+1)))



/*******************************MD070SD��ؼĴ�����ַ*******************************/


#define         BL_REG              0x0001         //��ַָ�򱳹����ȼĴ���
#define         START_ROW_REG       0x0002         //��ַָ����ʼ�е�ַ�Ĵ���
#define         START_COL_REG       0x0003         //��ַָ����ʼ�е�ַ�Ĵ���
#define         END_ROW_REG         0x0006         //��ַָ������е�ַ�Ĵ���
#define         END_COL_REG         0x0007         //��ַָ������е�ַ�Ĵ���
#define         PAGE_ADDR_REG       0x0004         //��ʾҳ�Ĵ���
#define         WR_PAGE_REG         0x0005         //��дҳ�Ĵ���
#define         SLEEP_REG           0x000B         //����״̬�Ĵ���
#define         SHOW_STATE_REG      0x000C         //��ʾ״̬ѡ��Ĵ���
#define         ADDR_ADD_REG        0x000D         //��ַָ���ַ��������Ĵ���
#define         DATA_CH_REG         0x000F         //��ַָ������ͨ��


/******************************************Һ����������غ�**************************************/
#define      MD070SD_SCREEN_START_X           0                      //��Ļ��ʼ�������
#define      MD070SD_SCREEN_START_Y           0                      //��Ļ��ʼ��������
#define      MD070SD_SCREEN_ROW_WIDTH        480                     //��Ļ��խ����Ŀ��
#define      MD070SD_SCREEN_COL_WIDTH        800                     //��Ļ�Ͽ���Ŀ��


/*************************************************RGB565��غ�***********************************************/

//RGB888תRGB565
#define      RGB888_TO_RGB565(r,g,b)         (u16)(((r & 0x1f)<<11) | ((g & 0x3f)<<5) | ((b & 0x1f)))

//������ɫ
#define      WHITE		 		                 0xFFFF	                 //��ɫ
#define      BLACK                         0x0000	                 //��ɫ 
#define      GREY                          0xF7DE	                 //��ɫ 
#define      BLUE                          0x001F	                 //��ɫ 
#define      BLUE2                         0x051F	                 //ǳ��ɫ 
#define      RED                           0xF800	                 //��ɫ 
#define      MAGENTA                       0xF81F	                 //����ɫ�����ɫ 
#define      GREEN                         0x07E0	                 //��ɫ 
#define      CYAN                          0x7FFF	                 //����ɫ����ɫ 
#define      YELLOW                        0xFFE0	                 //��ɫ 
#define      BRED                          0xF81F
#define      GRED                          0xFFE0
#define      GBLUE                         0x07FF

#define      GUI_BAK1                      0x0A0015
#define      GUI_BAK2                      0x000811

/******************************************��״�����ṹ��****************************************/

//Բ
typedef struct {
	u16 center_x,
	    center_y,
	    radius;
}CIRCLE;



/**********************************FSMC��MD070SD�����������***********************************/


void Fsmc_Config(void);
void MD070SD_Gpio_Config(void);
void MD070SD_Init(void);
void MD070SD_Write_Cmd(u16 cmd);
void MD070SD_Write_Data(u16 zdata);
u16  MD070SD_Read_Data(void);
u16 MD070SD_GetPixel(u16 posx , u16 posy);
void MD070SD_PutPixel(u16 posx , u16 posy , u16 color);
void MD070SD_Open_Window(u16 start_x , u16 start_y , u16 end_x , u16 end_y);
void MD070SD_Draw_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1 , u16 color);
void MD070SD_Draw_Circle(CIRCLE circle , u16 color , u8 mode);
void MD070SD_Draw_Line(u16 start_x , u16 start_y , u16 end_x , u16 end_y , u16 color);
void MD070SD_Draw_Cross(u16 posx , u16 posy);
void MD070SD_ShowChar_En(u16 pos_x , u16 pos_y , u8 zdata, FONT font);
void MD070SD_ShowString_En(u16 pos_x , u16 pos_y , u8 *zdata, FONT font);
void MD070SD_ShowChar_Ch(u16 pos_x , u16 pos_y , u8 *zdata, FONT font);
void MD070SD_ShowString_Ch(u16 pos_x , u16 pos_y , u8 *zdata, FONT font);
void MD070SD_Clear_Screen(u16 start_x , u16 start_y , u16 width , u16 height);
void MD070SD_Set_Txtcolor(u16 color);
void MD070SD_Set_Backcolor(u16 color);
void MD070SD_Set_BackLight(u8 BL_Value);



/* ͨ��FSMC��MD070SDоƬ���ж�д��������������ʵ�� */


//��MD070SDд�������
//2021.01.05
//���ߣ�����
//������cmd
__inline  void  MD070SD_Write_Cmd(u16 cmd)
{
	* MD070SD_CMD_ADDR = cmd;
}


//��MD070SDд������
//2021.01.05
//���ߣ�����
//������zdata
__inline void MD070SD_Write_Data(u16 zdata)
{
	* MD070SD_DATA_ADDR = zdata;
}


//��ȡMD070SD����
//2021.01.05
//���ߣ�����
//����ֵ����ȡ������
__inline u16 MD070SD_Read_Data(void)
{
	return * MD070SD_DATA_ADDR;
}


#endif


