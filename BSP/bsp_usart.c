/*******************************************
	*文件名 ：  bsp_usart.c
	*作   者：  王峰
	*修改时间： 2021.04.09
	*版   本：  v1.0
  *说   明：  串口配置源文件
*******************************************/



#include "bsp_usart.h"



#if DEBUG_USART1

/*
	*函数名： Usart1_Init()
	*功  能： 串口1初始化函数
	*参  数： BaudRate：波特率
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.02.19
*/
void Usart1_Init(u32 BaudRate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
    
	DEBUG_USART1_GPIO_APBxClkCmd(DEBUG_USART1_GPIO_CLK, ENABLE);  //使能GPIO时钟
	DEBUG_USART1_APBxClkCmd(DEBUG_USART1_CLK, ENABLE);            //使能USART1时钟
	
  //设置USART1的TX为复用推挽输出
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;               
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART1_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART1_TX_GPIO_PORT, &GPIO_InitStructure);
	
  //设置USART1的RX为浮空输入
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;    
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART1_RX_GPIO_PIN;
	GPIO_Init(DEBUG_USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;                  //设置USART1为发送接收模式
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//没有硬件流
	USART_InitStructure.USART_Parity=USART_Parity_No;                            //没有奇偶校验
	USART_InitStructure.USART_StopBits=USART_StopBits_1;                         //1个停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;                    //8个数据位
	USART_InitStructure.USART_BaudRate=BaudRate;                                 //设置波特率
  USART_Init(USART1, &USART_InitStructure);                                    //初始化USART1
	
	NVIC_InitStructure.NVIC_IRQChannel=DEBUG_USART1_IRQ;                         //设置中断通道为USART1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;                      //设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;                             //设置响应优先级为3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                                //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                                              //初始化中断
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);                                 //设置USART1为接收中断
	USART_Cmd(USART1, ENABLE);                                                   //使能USART1
}

#endif

#if DEBUG_USART2

/*
	*函数名： Usart2_Init()
	*功  能： 串口2初始化函数
	*参  数： BaudRate：波特率
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.04.09
*/
void Usart2_Init(u32 BaudRate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
    
	DEBUG_USART2_GPIO_APBxClkCmd(DEBUG_USART2_GPIO_CLK, ENABLE);  //使能GPIO时钟
	DEBUG_USART2_APBxClkCmd(DEBUG_USART2_CLK, ENABLE);            //使能USART2时钟
	
  //设置USART2的TX为复用推挽输出
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;               
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART2_TX_GPIO_PORT, &GPIO_InitStructure);
	
  //设置USART2的RX为浮空输入
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;    
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART2_RX_GPIO_PIN;
	GPIO_Init(DEBUG_USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;                  //设置USART2为发送接收模式
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//没有硬件流
	USART_InitStructure.USART_Parity=USART_Parity_No;                            //没有奇偶校验
	USART_InitStructure.USART_StopBits=USART_StopBits_1;                         //1个停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;                    //8个数据位
	USART_InitStructure.USART_BaudRate=BaudRate;                                 //设置波特率
  USART_Init(USART2, &USART_InitStructure);                                    //初始化USART2
	
	NVIC_InitStructure.NVIC_IRQChannel=DEBUG_USART2_IRQ;                          //设置中断通道为USART2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;                      //设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;                             //设置响应优先级为3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                                //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                                              //初始化中断
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);                                 //设置USART2为接收中断
	USART_Cmd(USART2, ENABLE);                                                   //使能USART2
}

#endif



#if DEBUG_USART3

/*
	*函数名： Usart3_Init()
	*功  能： 串口3初始化函数
	*参  数： BaudRate：波特率
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.04.09
*/
void Usart3_Init(u32 BaudRate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
    
	DEBUG_USART3_GPIO_APBxClkCmd(DEBUG_USART3_GPIO_CLK, ENABLE);  //使能GPIO时钟
	DEBUG_USART3_APBxClkCmd(DEBUG_USART3_CLK, ENABLE);            //使能USART3时钟
	
  //设置USART3的TX为复用推挽输出
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;               
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART3_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART3_TX_GPIO_PORT, &GPIO_InitStructure);
	
  //设置USART3的RX为浮空输入
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;    
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART3_RX_GPIO_PIN;
	GPIO_Init(DEBUG_USART3_RX_GPIO_PORT, &GPIO_InitStructure);
	
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;                  //设置USART3为发送接收模式
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//没有硬件流
	USART_InitStructure.USART_Parity=USART_Parity_No;                            //没有奇偶校验
	USART_InitStructure.USART_StopBits=USART_StopBits_1;                         //1个停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;                    //8个数据位
	USART_InitStructure.USART_BaudRate=BaudRate;                                 //设置波特率
  USART_Init(USART3, &USART_InitStructure);                                    //初始化USART3
	
	NVIC_InitStructure.NVIC_IRQChannel=DEBUG_USART3_IRQ;                          //设置中断通道为USART3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;                      //设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;                             //设置响应优先级为3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                                //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                                              //初始化中断
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);                                 //设置USART3为接收中断
	USART_Cmd(USART3, ENABLE);                                                   //使能USART3
}

#endif






/*
	*函数名： Usart_SendByte()
	*功  能： 串口发送字节函数
	*参  数： pUsart_x：串口结构体， zdata：字节数据
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.02.19
*/
void Usart_SendByte(USART_TypeDef* pUsart_x, u8 zdata)
{
    USART_SendData(pUsart_x, zdata);
    while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TXE) == RESET);                //等待发送数据寄存器空
}


/*
	*函数名： Usart_SendHalfWord()
	*功  能： 串口发送16位数据函数
	*参  数： pUsart_x：串口结构体， zdata：16位数据
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.02.19
*/
void Usart_SendHalfWord(USART_TypeDef* pUsart_x, u16 zdata)
{
    USART_SendData(pUsart_x, (u8)zdata&0x00ff);                                   //发送高8位数据                              
    while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TXE) == RESET);                
    USART_SendData(pUsart_x, (u8)zdata&0xff00);                                   //发送低8位数据
    while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TXE) == RESET);                
}


/*
	*函数名： Usart_SendArray()
	*功  能： 串口发送数组数据函数
	*参  数： pUsart_x：串口结构体， array：数据数组， Num：数据个数
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.02.19
*/
void Usart_SendArray(USART_TypeDef* pUsart_x, u8* array, u8 Num)
{  
    while(Num--)
    {
        USART_SendData(pUsart_x, *array);
        while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TXE) == RESET); 
        array++;
    }
    while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TC) == RESET);                  //等待发送完成
}


/*
	*函数名： Usart_SendString()
	*功  能： 串口发送字符串数据函数
	*参  数： pUsart_x：串口结构体， str：字符串
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.02.19
*/
void Usart_SendString(USART_TypeDef* pUsart_x, u8* str)
{
    while(*str != '\0')
    {
        USART_SendData(pUsart_x, *str);
        while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TXE) == RESET);
        str++;
    }
    while(USART_GetFlagStatus(pUsart_x, USART_FLAG_TC) == RESET);                  //等待发送完成
}

//重定向printf()到串口1（发送）
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
    return (ch);
}



//重定向scanf()到串口1（接收）
int fgetc(FILE *f)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET); //等到串口接收完成
    return (int)USART_ReceiveData(USART1);                        //返回接收的数据
}


#if DEBUG_USART1

/*
	*函数名： DEBUG_USART1_IRQHandler()
	*功  能： 串口1中断服务函数
	*参  数： 无
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.02.19
*/
void DEBUG_USART1_IRQHandler(void)
{
    u8 rData = 0;
    if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET) //读数据寄存器非空(即接收到数据)
    {
        rData = USART_ReceiveData(USART1);                  //将读取的数据保存到rData
        Usart_SendByte(USART1, rData);
    }
}

#endif


#if DEBUG_USART2

/*
	*函数名： DEBUG_USART2_IRQHandler()
	*功  能： 串口2中断服务函数
	*参  数： 无
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.04.09
*/
void DEBUG_USART2_IRQHandler(void)
{
    u8 rData = 0;
    if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET) //读数据寄存器非空(即接收到数据)
    {
        rData = USART_ReceiveData(USART2);                  //将读取的数据保存到rData
        Usart_SendByte(USART2, rData);
    }
}

#endif


#if DEBUG_USART3

/*
	*函数名： DEBUG_USART3_IRQHandler()
	*功  能： 串口3中断服务函数
	*参  数： 无
	*返回值： 无
	*作  者： 王峰
	*日  期： 2021.04.09
*/
void DEBUG_USART3_IRQHandler(void)
{
    u8 rData = 0;
    if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET) //读数据寄存器非空(即接收到数据)
    {
        rData = USART_ReceiveData(USART3);                  //将读取的数据保存到rData
        Usart_SendByte(USART3, rData);
    }
}

#endif


