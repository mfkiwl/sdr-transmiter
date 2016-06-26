#include "my_usart1.h"

void USART1_NVIC_Config(void);
void _ttywrch(int ch);

void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1

	//USART1端口配置
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //速度50MHz
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
 GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

 //USART1 初始化设置
USART_InitStructure.USART_BaudRate = 115200;//波特率设置
USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
USART_Init(USART1, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART1, ENABLE);  //使能串口1
	USART_ClearFlag(USART1,USART_IT_RXNE|USART_FLAG_TC);
}

char *itoa(int value, char *string, int radix)
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
	    *ptr = 0;
	    return string;
	}

	if (!value)
	{
	    *ptr++ = 0x30;
	    *ptr = 0;
	    return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
	    *ptr++ = '-';

	    /* Make the value positive. */
	    value *= -1;
	}

	for (i = 10000; i > 0; i /= 10)
	{
	    d = value / i;

	    if (d || flag)
	    {
	        *ptr++ = (char)(d + 0x30);
	        value -= (d * i);
	        flag = 1;
	    }
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;

} /* NCL_Itoa */

void myPrintf(uint8_t *Data,...)
{
	const char *s;
	int d;
	char buf[16];

	va_list ap;
	va_start(ap, Data);

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{
		if ( *Data == 0x5c )  //'\'
	{
	switch ( *++Data )
	{
		case 'r':							          //回车符
			USART_SendData(USART1, 0x0d);
			Data ++;
		break;

		case 'n':							          //换行符
			USART_SendData(USART1, 0x0a);
			Data ++;
		break;

		default:
			Data ++;
		break;
	}
	}
	else if ( *Data == '%')
	{									  //
	switch ( *++Data )
	{
		case 's':										  //字符串
			s = va_arg(ap, const char *);
	for ( ; *s; s++)
	{
		USART_SendData(USART1,*s);
		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
	}
		Data++;
		break;

	case 'd':										//十进制
	d = va_arg(ap, int);
	itoa(d, buf, 10);
	for (s = buf; *s; s++)
	{
		USART_SendData(USART1,*s);
		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
	}
	Data++;
	break;
		 default:
				Data++;
		    break;
	}
	} /* end of else if */
	else USART_SendData(USART1, *Data++);
	while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
	}
}
