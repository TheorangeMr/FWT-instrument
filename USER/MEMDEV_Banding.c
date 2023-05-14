/*******************************************
	*�ļ��� ��  MEMDEV_Banding.c
	*��   �ߣ�  WF
	*�޸�ʱ�䣺 2021.05.08
	*��   ����  v1.0
	*˵   ���� �ֶδ洢�豸Դ�ļ�
*******************************************/

#include "GUI.h"
#include <stdio.h>
#include "bsp_delay.h"
#include "bsp_lcd7_0.h"


extern uint16_t Batter_flag;
#define Battery_Color1  0x3c4d58
#define Battery_Color2  0x0269ab
#define Battery_Color3  0x00a6fb




/* �ֶδ洢�豸������������ṹ�� */
typedef struct {
  int x0;
  int y0;
  int x1;
  int y1;
  int i;
	float j;
} tDrawItContext;


/*******************************************************************
*
*       static code
*
********************************************************************
*/

/* 
	*��������_DrawIt_Speed
	*��  �ܣ������ٶ���ʾ����
	*��  �ߣ��޳�
	*��  ����void * pData��������������ṹ��
	*����ֵ����
	*ʱ  �䣺2022.05.18
*/
static void _DrawIt_Speed(void * pData) 
{
  tDrawItContext * pDrawItContext = (tDrawItContext *)pData;
  char buf[4] = { 0 };
	
	GUI_SetColor(GUI_BAK2);
	GUI_FillRoundedRect(pDrawItContext->x0 + 40, pDrawItContext->y0 + 50, pDrawItContext->x1-5,
	             pDrawItContext->y1,5);
	/* ��ʾ�ٶ� */
	GUI_SetBkColor(GUI_BAK2);
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(GUI_FONT_D64);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	sprintf(buf, "%d", pDrawItContext->i);
	if(pDrawItContext->i>=10)
	{
		GUI_DispStringHCenterAt(buf, pDrawItContext->x0+96,(pDrawItContext->y0 + (pDrawItContext->y1 - pDrawItContext->y0) / 2)-5);
	}
	else
	{
		GUI_DispStringHCenterAt(buf, (pDrawItContext->x0+(pDrawItContext->x1 - pDrawItContext->x0)/2+25),(pDrawItContext->y0 + (pDrawItContext->y1 - pDrawItContext->y0) / 2) - 5);
	}
}
/* 
	*��������_DrawIt_Oil
  *��  �ܣ�����������ʾ����
	*��  �ߣ��޳�
	*��  ����void * pData��������������ṹ��
	*����ֵ����
	*ʱ  �䣺2022.05.18
*/
static void _DrawIt_Oil(void * pData)
{
	 tDrawItContext * pDrawItContext = (tDrawItContext *)pData;
  char buf[4] = { 0 };
	
	GUI_SetColor(GUI_BAK2);
	GUI_FillRoundedRect(pDrawItContext->x0 + 10, pDrawItContext->y0 + 53, pDrawItContext->x1 - 30,
	             pDrawItContext->y1,5);
	/* ��ʾ����*/
	GUI_SetBkColor(GUI_BAK2);
	GUI_SetFont(GUI_FONT_D64);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	sprintf(buf, "%d", pDrawItContext->i);
	if(pDrawItContext->i>=10)
	{
		GUI_SetColor(GUI_WHITE);
		GUI_DispStringHCenterAt(buf, pDrawItContext->x0+62,(pDrawItContext->y0 + (pDrawItContext->y1 - pDrawItContext->y0) / 2)-5);
	}
	else
	{
		GUI_SetColor(GUI_RED);
		GUI_DispStringHCenterAt(buf, (pDrawItContext->x0+(pDrawItContext->x1 - pDrawItContext->x0)/2-10),(pDrawItContext->y0 + (pDrawItContext->y1 - pDrawItContext->y0) / 2) - 5);
	}
}
/* 
	*��������_DrawIt_Mileage
	*��  �ܣ����������ʾ����
	*��  �ߣ��޳�
	*��  ����void * pData��������������ṹ��
	*����ֵ����
	*ʱ  �䣺2022.08.26
*/
static void _DrawIt_Mileage(void * pData)
{
	 tDrawItContext * pDrawItContext = (tDrawItContext *)pData;
  char buf[10] = { 0 };
	
	GUI_SetColor(GUI_BAK2);
	GUI_FillRoundedRect(pDrawItContext->x0-25 , pDrawItContext->y0 , pDrawItContext->x1+10,
	             pDrawItContext->y1,4);
	/* ��ʾ���GUI_BAK2*/
	GUI_SetBkColor(GUI_BAK2);
	GUI_SetFont(GUI_FONT_24B_ASCII);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	sprintf(buf, "%.2f km", pDrawItContext->j);
	GUI_SetColor(GUI_WHITE);
	GUI_DispStringHCenterAt(buf, pDrawItContext->x0+40,(pDrawItContext->y0 + (pDrawItContext->y1 - pDrawItContext->y0) / 2)-10);
}

/* 
	*��������_DrawIt_Battery
	*��  �ܣ����Ƶ�����ʾ����
	*��  �ߣ��޳�
	*��  ����void * pData��������������ṹ��
	*����ֵ����
	*ʱ  �䣺2022.09.12
*/
static void _DrawIt_Battery(void * pData)
{
	tDrawItContext * pDrawItContext = (tDrawItContext *)pData;
	
	GUI_SetColor(Battery_Color2);
	GUI_FillRoundedRect(pDrawItContext->x0, pDrawItContext->y0, pDrawItContext->x1,
	             pDrawItContext->y1-4,3);
	GUI_FillRoundedRect(pDrawItContext->x1, pDrawItContext->y0+5, pDrawItContext->x1+3,
	             pDrawItContext->y1-9,1);	
	GUI_SetColor(GUI_BLACK);
	GUI_FillRoundedRect(pDrawItContext->x0+2, pDrawItContext->y0+2, pDrawItContext->x1-2,
	             pDrawItContext->y1-6,2);
	GUI_SetColor(Battery_Color1);
	GUI_FillRoundedRect(pDrawItContext->x0+4, pDrawItContext->y0+4, pDrawItContext->x0+18,
				 pDrawItContext->y1-8,1);
	GUI_FillRoundedRect(pDrawItContext->x0+20, pDrawItContext->y0+4, pDrawItContext->x0+34,
				 pDrawItContext->y1-8,1);
	GUI_FillRoundedRect(pDrawItContext->x0+36, pDrawItContext->y0+4, pDrawItContext->x0+50,
				 pDrawItContext->y1-8,1);
	GUI_FillRoundedRect(pDrawItContext->x0+52, pDrawItContext->y0+4, pDrawItContext->x0+64,
				 pDrawItContext->y1-8,1);
	GUI_FillRoundedRect(pDrawItContext->x0+66, pDrawItContext->y0+4, pDrawItContext->x0+78,
				 pDrawItContext->y1-8,1);
	GUI_FillRoundedRect(pDrawItContext->x0+80, pDrawItContext->y0+4, pDrawItContext->x0+92,
				 pDrawItContext->y1-8,1);
//Batter_flag				 
	switch((int)pDrawItContext->j)
	{
		case(1):
		{
			GUI_SetColor(GUI_RED);
			GUI_FillRoundedRect(pDrawItContext->x0+4, pDrawItContext->y0+4, pDrawItContext->x0+18,
						 pDrawItContext->y1-8,1);
			break;
		}
		case(2):
		{
			GUI_SetColor(Battery_Color3);
			GUI_FillRoundedRect(pDrawItContext->x0+4, pDrawItContext->y0+4, pDrawItContext->x0+18,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+20, pDrawItContext->y0+4, pDrawItContext->x0+34,
						 pDrawItContext->y1-8,1);
			break;
		}
		case(3):
		{
			GUI_SetColor(Battery_Color3);
			GUI_FillRoundedRect(pDrawItContext->x0+4, pDrawItContext->y0+4, pDrawItContext->x0+18,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+20, pDrawItContext->y0+4, pDrawItContext->x0+34,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+36, pDrawItContext->y0+4, pDrawItContext->x0+50,
						 pDrawItContext->y1-8,1);
			break;
		}
		case(4):
		{
			GUI_SetColor(Battery_Color3);
			GUI_FillRoundedRect(pDrawItContext->x0+4, pDrawItContext->y0+4, pDrawItContext->x0+18,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+20, pDrawItContext->y0+4, pDrawItContext->x0+34,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+36, pDrawItContext->y0+4, pDrawItContext->x0+50,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+52, pDrawItContext->y0+4, pDrawItContext->x0+64,
						 pDrawItContext->y1-8,1);
			break;
		}
		case(5):
		{
			GUI_SetColor(Battery_Color3);
			GUI_FillRoundedRect(pDrawItContext->x0+4, pDrawItContext->y0+4, pDrawItContext->x0+18,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+20, pDrawItContext->y0+4, pDrawItContext->x0+34,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+36, pDrawItContext->y0+4, pDrawItContext->x0+50,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+52, pDrawItContext->y0+4, pDrawItContext->x0+64,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+66, pDrawItContext->y0+4, pDrawItContext->x0+78,
						 pDrawItContext->y1-8,1);

			break;
		}
		case(6):
		{
			GUI_SetColor(Battery_Color3);
			GUI_FillRoundedRect(pDrawItContext->x0+4, pDrawItContext->y0+4, pDrawItContext->x0+18,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+20, pDrawItContext->y0+4, pDrawItContext->x0+34,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+36, pDrawItContext->y0+4, pDrawItContext->x0+50,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+52, pDrawItContext->y0+4, pDrawItContext->x0+64,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+66, pDrawItContext->y0+4, pDrawItContext->x0+78,
						 pDrawItContext->y1-8,1);
			GUI_FillRoundedRect(pDrawItContext->x0+80, pDrawItContext->y0+4, pDrawItContext->x0+92,
			       pDrawItContext->y1-8,1);
			break;
		}
		default:
			break;
	}
}

static void _DrawIt_Network(void * pData)
{
	char buf[5] = { 0 };
	tDrawItContext * pDrawItContext = (tDrawItContext *)pData;
//	GUI_SetColor(Battery_Color2);
//	GUI_FillRoundedRect(pDrawItContext->x1+23, pDrawItContext->y0, pDrawItContext->x1+28,
//						 pDrawItContext->y1,1);                                                        //5GUI_BAK2
//	GUI_FillRoundedRect(pDrawItContext->x1+16, pDrawItContext->y0+5, pDrawItContext->x1+21,
//						 pDrawItContext->y1,1);
//	GUI_FillRoundedRect(pDrawItContext->x1+9, pDrawItContext->y0+10, pDrawItContext->x1+14,
//						 pDrawItContext->y1,1); 
//	GUI_FillRoundedRect(pDrawItContext->x1+2, pDrawItContext->y0+15, pDrawItContext->x1+7,
//						 pDrawItContext->y1,1);
//	GUI_FillRoundedRect(pDrawItContext->x0, pDrawItContext->y0+20, pDrawItContext->x1,       //1
//						 pDrawItContext->y1,1);
	switch((int)pDrawItContext->j)
	{
		case(0):
		{
			GUI_SetColor(GUI_BAK2);
			GUI_FillRoundedRect(pDrawItContext->x1+23, pDrawItContext->y0, pDrawItContext->x1+28,
								 pDrawItContext->y1,1);                                                        //5
			GUI_FillRoundedRect(pDrawItContext->x1+16, pDrawItContext->y0+5, pDrawItContext->x1+21,
								 pDrawItContext->y1,1);
			GUI_FillRoundedRect(pDrawItContext->x1+9, pDrawItContext->y0+10, pDrawItContext->x1+14,
								 pDrawItContext->y1,1); 
			GUI_FillRoundedRect(pDrawItContext->x1+2, pDrawItContext->y0+15, pDrawItContext->x1+7,
								 pDrawItContext->y1,1);
			GUI_FillRoundedRect(pDrawItContext->x0, pDrawItContext->y0+20, pDrawItContext->x1,       //1
								 pDrawItContext->y1,1);
			break;
		}
		case(1):
		{
			GUI_SetColor(GUI_BAK2);
			GUI_FillRoundedRect(pDrawItContext->x1+23, pDrawItContext->y0, pDrawItContext->x1+28,
								 pDrawItContext->y1,1);                                                        //5
			GUI_FillRoundedRect(pDrawItContext->x1+16, pDrawItContext->y0+5, pDrawItContext->x1+21,
								 pDrawItContext->y1,1);
			GUI_FillRoundedRect(pDrawItContext->x1+9, pDrawItContext->y0+10, pDrawItContext->x1+14,
								 pDrawItContext->y1,1); 
			GUI_FillRoundedRect(pDrawItContext->x1+2, pDrawItContext->y0+15, pDrawItContext->x1+7,
								 pDrawItContext->y1,1);
			GUI_SetColor(GUI_WHITE);			
			GUI_FillRoundedRect(pDrawItContext->x0, pDrawItContext->y0+20, pDrawItContext->x1,       //1
								 pDrawItContext->y1,1);	
			break;			
		}
		case(2):
		{
			GUI_SetColor(GUI_BAK2);
			GUI_FillRoundedRect(pDrawItContext->x1+23, pDrawItContext->y0, pDrawItContext->x1+28,
								 pDrawItContext->y1,1);                                                        //5
			GUI_FillRoundedRect(pDrawItContext->x1+16, pDrawItContext->y0+5, pDrawItContext->x1+21,
								 pDrawItContext->y1,1);
			GUI_FillRoundedRect(pDrawItContext->x1+9, pDrawItContext->y0+10, pDrawItContext->x1+14,
								 pDrawItContext->y1,1);
			GUI_SetColor(GUI_WHITE);			
			GUI_FillRoundedRect(pDrawItContext->x1+2, pDrawItContext->y0+15, pDrawItContext->x1+7,
								 pDrawItContext->y1,1);
			GUI_FillRoundedRect(pDrawItContext->x0, pDrawItContext->y0+20, pDrawItContext->x1,       //1
								 pDrawItContext->y1,1);
			break;
		}
		case(3):
		{
			GUI_SetColor(GUI_BAK2);
			GUI_FillRoundedRect(pDrawItContext->x1+23, pDrawItContext->y0, pDrawItContext->x1+28,
								 pDrawItContext->y1,1);                                                        //5
			GUI_FillRoundedRect(pDrawItContext->x1+16, pDrawItContext->y0+5, pDrawItContext->x1+21,
								 pDrawItContext->y1,1);
			GUI_SetColor(GUI_WHITE);
			GUI_FillRoundedRect(pDrawItContext->x1+9, pDrawItContext->y0+10, pDrawItContext->x1+14,
								 pDrawItContext->y1,1); 
			GUI_FillRoundedRect(pDrawItContext->x1+2, pDrawItContext->y0+15, pDrawItContext->x1+7,
								 pDrawItContext->y1,1);
			GUI_FillRoundedRect(pDrawItContext->x0, pDrawItContext->y0+20, pDrawItContext->x1,       //1
								 pDrawItContext->y1,1);
			break;
		}
		case(4):
		{
			GUI_SetColor(GUI_BAK2);
			GUI_FillRoundedRect(pDrawItContext->x1+23, pDrawItContext->y0, pDrawItContext->x1+28,    //5
								 pDrawItContext->y1,1);
			GUI_SetColor(GUI_WHITE);			
			GUI_FillRoundedRect(pDrawItContext->x1+16, pDrawItContext->y0+5, pDrawItContext->x1+21,
								 pDrawItContext->y1,1);
			GUI_FillRoundedRect(pDrawItContext->x1+9, pDrawItContext->y0+10, pDrawItContext->x1+14,
								 pDrawItContext->y1,1); 
			GUI_FillRoundedRect(pDrawItContext->x1+2, pDrawItContext->y0+15, pDrawItContext->x1+7,
								 pDrawItContext->y1,1);
			GUI_FillRoundedRect(pDrawItContext->x0, pDrawItContext->y0+20, pDrawItContext->x1,       //1
								 pDrawItContext->y1,1);
			break;
		}
		case(5):
		{
			GUI_SetColor(GUI_WHITE);
			GUI_FillRoundedRect(pDrawItContext->x1+23, pDrawItContext->y0, pDrawItContext->x1+28,
								 pDrawItContext->y1,1);                                                        //5
			GUI_FillRoundedRect(pDrawItContext->x1+16, pDrawItContext->y0+5, pDrawItContext->x1+21,
								 pDrawItContext->y1,1);
			GUI_FillRoundedRect(pDrawItContext->x1+9, pDrawItContext->y0+10, pDrawItContext->x1+14,
								 pDrawItContext->y1,1); 
			GUI_FillRoundedRect(pDrawItContext->x1+2, pDrawItContext->y0+15, pDrawItContext->x1+7,
								 pDrawItContext->y1,1);
			GUI_FillRoundedRect(pDrawItContext->x0, pDrawItContext->y0+20, pDrawItContext->x1,       //1
								 pDrawItContext->y1,1);
			break;
		}		
		default:
			break;
	}

	GUI_SetColor(GUI_BAK2);
	GUI_FillRoundedRect(pDrawItContext->x0-5, pDrawItContext->y0, pDrawItContext->x1+5,     
						 pDrawItContext->y1-18,1);
	GUI_SetFont(GUI_FONT_8_ASCII);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	sprintf(buf, "%s", "4G");
	GUI_SetColor(GUI_WHITE);
	GUI_DispStringHCenterAt(buf, pDrawItContext->x0+2,pDrawItContext->y0+2); 

}


/* 
	*��������_DemoBandingMemdev_Speed
	*��  �ܣ��ֶδ洢�豸�����ٶ���ʾ����
	*��  �ߣ�WF
	*��  ����u8 zdata���ٶ�
	*����ֵ����
	*ʱ  �䣺2021.05.08
*/
void _DemoBandingMemdev_Speed(u8 zdata) 
{
	/* ������ʾ������� */
	tDrawItContext DrawItContext = {40, 130, 190, 256, zdata};
  
	GUI_RECT Rect = {40, 130, 190, 256};
	/* Use banding memory device for drawing */
	GUI_MEMDEV_Draw(&Rect, &_DrawIt_Speed, &DrawItContext, 0, 0);
}

void _DemoBandingMemdev_Oil(u8 zdata)
{
		/* ������ʾ������� */
	tDrawItContext DrawItContext = {600, 130, 750, 256, zdata};
  
	GUI_RECT Rect = {600, 130, 750, 256};
	/* Use banding memory device for drawing */
	GUI_MEMDEV_Draw(&Rect, &_DrawIt_Oil, &DrawItContext, 0, 0);
}

void _DemoBandingMemdev_Mileage(float zdata)
{
		/* ������ʾ������� */
	tDrawItContext DrawItContext = {600, 420, 680, 440, 0,zdata};
  
//	GUI_RECT Rect = {590, 425, 700, 450};
	GUI_RECT Rect = {500, 420, 700, 440};
	/* Use banding memory device for drawing */
	GUI_MEMDEV_Draw(&Rect, &_DrawIt_Mileage, &DrawItContext, 0, 0);
}


void _DemoBandingMemdev_Battery(float zdata)
{
		/* ������ʾ������� */
	tDrawItContext DrawItContext = {50, 425, 146, 450, 0,zdata};
	GUI_RECT Rect = {50, 425, 160, 451};
	/* Use banding memory device for drawing */
	GUI_MEMDEV_Draw(&Rect, &_DrawIt_Battery, &DrawItContext, 0, 0);
}


void _DemoBandingMemdev_Network(float zdata)
{
		/* ������ʾ������� */
	tDrawItContext DrawItContext = {10, 8, 14, 38, 0,zdata};
	GUI_RECT Rect = {2, 5, 100, 100};
	/* Use banding memory device for drawing */
	GUI_MEMDEV_Draw(&Rect, &_DrawIt_Network, &DrawItContext, 0, 0);
}


/*************************** End of file ****************************/

