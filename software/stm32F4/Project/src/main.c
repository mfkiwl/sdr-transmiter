#include "stm32f4xx.h"
#include "my_usart1.h"
#include "tim.h"
#include "externalDAC.h"
#include "ADF4351.h"

int main()
{
	USART1_Init();
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	myPrintf("SYSCLK_Frequency=%d, HCLK_Frequency=%d, PCLK1_Frequency=%d, PCLK2_Frequency=%d\r\n",RCC_Clocks.SYSCLK_Frequency/1000000,\
		RCC_Clocks.HCLK_Frequency/1000000, RCC_Clocks.PCLK1_Frequency/1000000, RCC_Clocks.PCLK2_Frequency/1000000);

	ADF4351_Initiate();
	gpsL1();
	// ADF_SetFre();
	//Frequency_100MHz();
	//F35();
	//
	// externalDACInit();
	myPrintf("I init ok!\r\n");
	// TIM1_Init();

	while(1);
}
