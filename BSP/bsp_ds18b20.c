#include "bsp_ds18b20.h"
#include "bsp_delay.h"
#include "stdio.h"

//u8 DS18B20_ID[8] = {0x28, 0xff, 0x64, 0x6, 0xf9, 0x56, 0xb7, 0x20};
u8 DS18B20_ID[8] = {0x28, 0xff, 0x64, 0x6, 0xf9, 0x2d, 0x17, 0x1a};

/* 
	*��������Ds18b20_GPIO_Config()
	*��  �ܣ�DS18B20�������ú���
	*��  �ߣ�WF
	*��  ������
	*����ֵ����
	*ʱ  �䣺2021.04.01
*/
void Ds18b20_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
	DS18B20_DQ_GPIO_APBxClkCmd(DS18B20_DQ_GPIO_CLK, ENABLE);	  //ʹ��DS18B20_DQ�˿�ʱ��
	
	/* DS18B20_DQ�˿����� */
	GPIO_InitStructure.GPIO_Pin = DS18B20_DQ_GPIO_PIN;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	          
	GPIO_Init(DS18B20_DQ_GPIO_PORT, &GPIO_InitStructure);			  
	
	GPIO_SetBits(DS18B20_DQ_GPIO_PORT, DS18B20_DQ_GPIO_PIN);
}


/* 
	*��������Ds18b20_Reset()
	*��  �ܣ�DS18B20��λ����
	*��  �ߣ�WF
	*��  ������
	*����ֵ������1����λ�ɹ��� ����0����λʧ��
	*ʱ  �䣺2021.04.01
*/
u8 Ds18b20_Reset(void)
{
	DS18B20_DQ_OUT();
	DS18B20_DQ_RESET();           //������������480us
	Delay_Us(480);     
	DS18B20_DQ_SET();             //�����ͷ�����
	Delay_Us(60);                 //DS18B20�ȴ�60us����׼��

	DS18B20_DQ_IN();
	if(DS18B20_DQ_READ() == 0)    //DS18B20������������λ�ɹ�
	{
		Delay_Us(480);
		return 1;
	}
	
	return 0;                     //��λʧ��
}



/* 
	*��������Ds18b20_Init()
	*��  �ܣ�DS18B20��ʼ������
	*��  �ߣ�WF
	*��  ������
	*����ֵ������1����λ�ɹ��� ����0����λʧ��
	*ʱ  �䣺2021.04.01
*/
u8 Ds18b20_Init(void)
{
	u8 flag = 0;
	u8 times = 0;
	
	Ds18b20_GPIO_Config();
	
//  printf("1\r\n");
	
	while(flag == 0 && times<1) //��ʱ���
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
		return 0;   //��ʱ����ʼ��ʧ��
	}
	else 
	{
		return 1;   //��ʼ���ɹ�
	}
}


/* 
	*��������Ds18b20_WriteByte()
	*��  �ܣ�DS18B20д�ֽں�������
	*��  �ߣ�WF
	*��  ����u8 zdata����д���ֽ�����
	*����ֵ����
	*ʱ  �䣺2021.04.01
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
			DS18B20_DQ_SET();      //�����ݡ�1���������ߣ��ȴ�DS18B20��ʼ����
		}
		else
		{
			DS18B20_DQ_RESET();    //�����ݡ�0���������ߣ��ȴ�DS18B20��ʼ����
		}
		Delay_Us(60);        
		DS18B20_DQ_SET();        //����60us�����һλдʱ϶�������ͷ�����
		Delay_Us(2);
		zdata >>= 1;
	}
}


/* 
	*��������Ds18b20_ReadByte()
	*��  �ܣ�DS18B20���ֽں���
	*��  �ߣ�WF
	*��  ������
	*����ֵ����ȡ���ֽ�����
	*ʱ  �䣺2021.04.01
*/
u8 Ds18b20_ReadByte(void)
{
	u8 i = 0;
	u8 tempdata = 0;
	
	for(i = 0; i < 8; i++)
	{
		DS18B20_DQ_OUT();
		tempdata >>= 1;
		DS18B20_DQ_RESET();          //������������
		Delay_Us(2);
		DS18B20_DQ_SET();            //����1us�������ͷ�����
		
		DS18B20_DQ_IN();
		if(DS18B20_DQ_READ() == 1)   //������15us�ڲ���DS18B20�͵����ߵ�����
		{
				 tempdata |= 0x80;
		}
		Delay_Us(60);
	}
	return tempdata;
}


/* 
	*��������Ds18b20_ReadSingleTemp()
	*��  �ܣ�DS18B20��ȡ�����¶Ⱥ���
	*��  �ߣ�WF
	*��  ����u8 Num��DS18B20���
	*����ֵ����ȡ���¶�����
	*ʱ  �䣺2021.04.01
*/
u16 Ds18b20_ReadSingleTemp(u8 Num)
{
	u8 LByte = 0, HByte = 0;
	u16 temperature = 0;

	Ds18b20_Reset();
	Ds18b20_MatchRom(DS18B20_ID);                 //ƥ��ROM
//	Ds18b20_WriteByte(DS18B20_SKIP_ROM);
	Ds18b20_WriteByte(DS18B20_CONVERT);           //�����¶�ת��
	Delay_Ms(5);                                  //��ʱ��С�ͷֱ����йأ�����ϵͳ����ʱ���Խ��������
	
	Ds18b20_Reset();
	Ds18b20_MatchRom(DS18B20_ID);                 //ƥ��ROM
//	Ds18b20_WriteByte(DS18B20_SKIP_ROM);	
	Ds18b20_WriteByte(DS18B20_READ_REG);          //���Ͷ����¶�ָ��
	
	LByte = Ds18b20_ReadByte();                   //��ȡ�¶ȵ�8λ
	HByte = Ds18b20_ReadByte();                   //��ȡ�¶ȸ�8λ

	
	if((HByte&0xf8) == 0xf8)                      //����Ǹ��¶�ֵ��Ҫ������ת����ԭ��
	{
		temperature = (HByte<<8) + LByte;
		temperature -= 1;
		temperature = ~temperature;
		temperature = temperature*0.0625 * 10;
	}
	else                                          //��������¶�
	{
		temperature = ((HByte&0x07)*256 + LByte) * 0.0625 * 10; //����һλС��
	}
	
	return temperature;
}


/* 
	*��������Ds18b20_ReadMultiTemp()
	*��  �ܣ�DS18B20��ȡ����¶Ⱥ���
	*��  �ߣ�WF
	*��  ����u16* TmpBuffer���¶����ݻ���Buffer
	*����ֵ����
	*ʱ  �䣺2021.04.08
*/
void Ds18b20_ReadMultiTemp(u16* TempBuffer)
{
	u8 LByte = 0, HByte = 0;
	u16 temperature = 0;
	u8 i = 0;
	
	Ds18b20_Reset();
	Ds18b20_WriteByte(DS18B20_SKIP_ROM);          //����ROM
	Ds18b20_WriteByte(DS18B20_CONVERT);           //ͳһ�����¶�ת��
	Delay_Ms(1000);             
	
	/* ѭ����ȡÿ��DS18B20���¶� */
	for(i = 0; i < DS18B20_NUM; i++)
	{
		Ds18b20_Reset();
		//Ds18b20_MatchRom(BAT_Module_DS18B20_ID[i]);   //ƥ��ROM
		Ds18b20_WriteByte(DS18B20_READ_REG);          //���Ͷ����¶�ָ��
		
		LByte = Ds18b20_ReadByte(); //��ȡ�¶ȵ�8λ
		HByte = Ds18b20_ReadByte(); //��ȡ�¶ȸ�8λ
		
		if((HByte&0xf8) == 0xf8)              //����Ǹ��¶�ֵ��Ҫ������ת����ԭ��
		{
			temperature = (HByte<<8) + LByte;
			temperature -= 1;
			temperature = ~temperature;
			temperature = temperature*0.0625;
		}
		else  //��������¶�
		{
			temperature = ((HByte&0x07)*256 + LByte) * 0.0625 * 10; //����һλС��
		}
		
		TempBuffer[i] = temperature;
	}
}


/* 
	*��������Ds18b20_ReadRom()
	*��  �ܣ�DS18B20��ȡRom����
	*��  �ߣ�WF
	*��  ����u8 *zdata������64λROM���е�����
	*����ֵ����
	*ʱ  �䣺2021.04.01
*/
void Ds18b20_ReadRom(u8 *zdata)
{
	u8 i = 0;
	Ds18b20_Reset();
	Ds18b20_WriteByte(DS18B20_READ_ROM); //���Ͷ�ROMȡָ��

	/* �ӻ�����64λROM���и�������ȡ */
	for(i = 0; i < 8; i++)
	{
		zdata[i] = Ds18b20_ReadByte();
	}
}


/* 
	*��������Ds18b20_MatchRom()
	*��  �ܣ�DS18B20ƥ��ROM����
	*��  �ߣ�WF
	*��  ����u8 *zdata����ƥ���64λROM��������
	*����ֵ����
	*ʱ  �䣺2021.04.01
*/
void Ds18b20_MatchRom(u8 *zdata)
{
	u8 i = 0;
	
	Ds18b20_WriteByte(DS18B20_MATCH_ROM); //����ƥ��ROMָ��
	
	/* ��������64λROM���е��ӻ�����ƥ�� */
	for(i = 0; i < 8; i++)
	{
		Ds18b20_WriteByte(zdata[i]);
	}
}


