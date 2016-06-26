#ifndef __ADF4351_H
#define __ADF4351_H

#include "stm32f4xx.h"

#define SET_LE()		GPIO_SetBits(GPIOB, GPIO_Pin_13)	//P4.3->LE
#define CLR_LE()		GPIO_ResetBits(GPIOB, GPIO_Pin_13)

#define	SET_SCL()		GPIO_SetBits(GPIOB, GPIO_Pin_15)	//P4.4->SCL
#define	CLR_SCL()		GPIO_ResetBits(GPIOB, GPIO_Pin_15)

#define SET_DATA()		GPIO_SetBits(GPIOB, GPIO_Pin_14)	//P4.5->DATA
#define CLR_DATA()		GPIO_ResetBits(GPIOB, GPIO_Pin_14)

#define SET_CE()		GPIO_SetBits(GPIOB, GPIO_Pin_12)	//P4.3->CE
#define CLR_CE()		GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define ADF_R0 0
#define ADF_R1 1
#define ADF_R2 2
#define ADF_R3 3
#define ADF_R4 4
#define ADF_R5 5
#define RF_div1 0
#define RF_div2 1
#define RF_div4 2
#define RF_div8 3
#define RF_div16 4
#define RF_div32 5
#define RF_div64 6

void delay (int length);

void ADF4351_Initiate(void);
void WriteToADF4350(unsigned char count, unsigned char *buf);
void F35(void);
void Frequency_35MHz(void);
void Frequency_100MHz(void);
void Frequency_200MHz(void);
void Frequency_300MHz(void);
void Frequency_400MHz(void);
void Frequency_500MHz(void);
void Frequency_600MHz(void);
void Frequency_700MHz(void);
void Frequency_800MHz(void);
void Frequency_900MHz(void);
void ADF_SetFre(void);
void gpsL1();

#endif













// #ifndef _ADF4351_H_
// #define _ADF4351_H_
// #include "stm32f4xx.h"
//
// #define ADF4351_CLK_SET GPIO_SetBits(GPIOB,15)
// #define ADF4351_CLK_RESET GPIO_ResetBits(GPIOB,15)
// #define ADF4351_OUTPUT_DATA_SET GPIO_SetBits(GPIOB,14)
// #define ADF4351_OUTPUT_DATA_RESET GPIO_ResetBits(GPIOB,14)
// #define ADF4351_LE_SET GPIO_SetBits(GPIOB,13)
// #define ADF4351_LE_RESET GPIO_ResetBits(GPIOB,13)
// #define ADF4351_CE_SET GPIO_SetBits(GPIOB,12)
// #define ADF4351_CE_RESET GPIO_ResetBits(GPIOB,12)
//
// void ADF4351Init(void);
// void WriteToADF4351(uint8_t count, uint8_t *buf);
// void WriteOneRegToADF4351(uint32_t Regster);
// void ADF4351_Init_some(void);
// void ADF4351WriteFreq(float Fre);		//	(xx.x) M Hz
//
// #endif
