#ifndef __MY_USART1_H_
#define __MY_USART1_H_

#include <stdarg.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include <stdlib.h>

void USART1_Init();
void myPrintf(uint8_t *Data,...);

#endif
