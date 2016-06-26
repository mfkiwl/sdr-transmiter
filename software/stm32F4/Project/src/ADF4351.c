#include "ADF4351.h"

u8 buf[4];


//д��32���ֽ�
void ADF4351_Write(u32 adf_dat)
{
	u8 i;

	CLR_LE();
	for (i = 0; i < 32; i++)
	{
		if ((adf_dat & 0x80000000) == 0x80000000)
			SET_DATA();
		else
			CLR_DATA();
		CLR_SCL();
		SET_SCL();
		CLR_SCL();
		adf_dat <<= 1;
	}
	SET_LE();
	delay(1);
	CLR_LE();
}

void delay (int length)
{
	int i;
	i = length * 200 ;
	while (i >0)
	i--;
}

void ADF4351_Initiate(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE); ;
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

  SET_CE();
	// GPIO_ResetBits(GPIOB, GPIO_Pin_1);	//PDRF��Ϊ0
}




//---------------------------------
//void WriteToADF4350(unsigned char count,unsigned char *buf);
//---------------------------------
//Function that writes to the ADF4350 via the SPI port.
//--------------------------------------------------------------------------------


void WriteToADF4350(unsigned char count, unsigned char *buf)
{
	unsigned	char	ValueToWrite = 0;
    unsigned	char	i = 0;
	unsigned	char	j = 0;


	delay(1);
	CLR_SCL();
	CLR_LE();
	delay(1);

	for(i=count;i>0;i--)
 	{
	 	ValueToWrite = *(buf + i - 1);
		for(j=0; j<8; j++)
		{
			if(0x80 == (ValueToWrite & 0x80))
			{
				SET_DATA();	  //Send one to SDO pin
			}
			else
			{
				CLR_DATA();	  //Send zero to SDO pin
			}
			delay(1);
			SET_SCL();
			delay(1);
			ValueToWrite <<= 1;	//Rotate data
			CLR_SCL();
		}
	}
	CLR_DATA();
	delay(1);
	SET_LE();
	delay(1);
	CLR_LE();
}


//---------------------------------
//void ReadFromADF4350(unsigned char count,unsigned char *buf)
//---------------------------------
//Function that reads from the ADF4350 via the SPI port.
//--------------------------------------------------------------------------------
void ReadFromADF4350(unsigned char count, unsigned char *buf)
{
	unsigned	char	i = 0;
	unsigned	char	j = 0;
	unsigned	int  	iTemp = 0;
	unsigned	char  	RotateData = 0;



	delay(1);
	CLR_SCL();
	CLR_LE();
	delay(1);

	for(j=count; j>0; j--)
	{
		for(i=0; i<8; i++)
		{
			RotateData <<= 1;		//Rotate data
			delay(1);
//			iTemp = GP4DAT;			//Read DATA of ADF4350
			SET_SCL();
			if(0x00000020 == (iTemp & 0x00000020))
			{
				RotateData |= 1;
			}
			delay(1);
			CLR_SCL();
		}
		*(buf + j - 1)= RotateData;
	}
	delay(1);
	SET_LE();
	delay(1);
	CLR_LE();
}

void gpsL1()
{
	ADF4351_Write(0x580005);
	ADF4351_Write(0x9A003C);
	ADF4351_Write(0x4B3);
	ADF4351_Write(0x14E42);
	ADF4351_Write(0x8008191);
	ADF4351_Write(0x4E80D8);
}
void F35(){
u8 buf[4] = {0,0,0,0};

buf[3] = 0x00;				//�˴����ù̶�����35M
buf[2] = 0x58;
buf[1] = 0x00;				//write communication register 0x00580005 to control the progress
buf[0] = 0x05;				//to write Register 5 to set digital lock detector
WriteToADF4350(4,buf);

buf[3] = 0x00;
buf[2] = 0xec;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
buf[1] = 0x80;				//(DB11=0)VCO powerd up;
buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
WriteToADF4350(4,buf);

buf[3] = 0x00;
buf[2] = 0x00;
buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
buf[0] = 0xB3;
WriteToADF4350(4,buf);

buf[3] = 0x00;
buf[2] = 0x01;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
buf[1] = 0x0E;				//(DB8=0)enable fractional-N digital lock detect;
buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1

buf[3] = 0x00;
buf[2] = 0x00;
buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
buf[0] = 0x29;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync
WriteToADF4350(4,buf);	   //nor the spurious optimization functions are being used
							 //(DB27=1)prescaler value is 8/9

buf[3] = 0x00;
buf[2] = 0x2c;
buf[1] = 0x80;
buf[0] = 0x18;				//(DB14-3:0H)FRAC value is 0;
WriteToADF4350(4,buf);		//(DB30-15:140H)INT value is 320;
}
void Frequency_100MHz(void)
{

	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0xDC;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x4E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x08;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x11;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync
	WriteToADF4350(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x40;
	buf[1] = 0x00;
 	buf[0] = 0x00;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4350(4,buf);		//(DB30-15:140H)INT value is 320;

}

void Frequency_200MHz(void)
{

	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0xCC;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x4E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x08;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x11;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync
	WriteToADF4350(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x40;
	buf[1] = 0x00;
 	buf[0] = 0x00;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4350(4,buf);		//(DB30-15:140H)INT value is 320;

}

void Frequency_300MHz(void)
{

	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0xBC;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x4E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x08;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x11;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync
	WriteToADF4350(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x30;
	buf[1] = 0x00;
 	buf[0] = 0x00;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4350(4,buf);		//(DB30-15:140H)INT value is 320;

}

void Frequency_400MHz(void)
{

	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0xBC;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x4E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x08;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x11;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync
	WriteToADF4350(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x40;
	buf[1] = 0x00;
 	buf[0] = 0x00;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4350(4,buf);		//(DB30-15:140H)INT value is 320;

}

void Frequency_500MHz(void)
{

	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0xCC;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x4E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x08;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x11;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync
	WriteToADF4350(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x50;
	buf[1] = 0x00;
 	buf[0] = 0x00;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4350(4,buf);		//(DB30-15:140H)INT value is 320;

}

void Frequency_600MHz(void)
{

	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0xAC;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x4E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x08;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x11;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync
	WriteToADF4350(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x30;
	buf[1] = 0x00;
 	buf[0] = 0x00;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4350(4,buf);		//(DB30-15:140H)INT value is 320;

}

void Frequency_700MHz(void)
{

	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0xAC;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x4E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x08;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x11;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync
	WriteToADF4350(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x38;
	buf[1] = 0x00;
 	buf[0] = 0x00;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4350(4,buf);		//(DB30-15:140H)INT value is 320;

}

void Frequency_800MHz(void)
{

	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0xAC;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x4E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x08;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x11;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync
	WriteToADF4350(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x40;
	buf[1] = 0x00;
 	buf[0] = 0x00;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4350(4,buf);		//(DB30-15:140H)INT value is 320;

}

void Frequency_900MHz(void)
{

	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0xAC;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x4E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x08;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x11;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync
	WriteToADF4350(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x48;
	buf[1] = 0x00;
 	buf[0] = 0x00;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4350(4,buf);		//(DB30-15:140H)INT value is 320;

}

void Frequency_35MHz(void)
{

	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0xDc;	//EC			//(DB23=1)The signal is taken from the VCO directly;(DB22-20:6H)the RF divider is 64;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4350(4,buf);

	buf[3] = 0x00;
	buf[2] = 0x00;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x4E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x08;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x29;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync
	WriteToADF4350(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x2C;
	buf[1] = 0x80;
 	buf[0] = 0x18;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4350(4,buf);		//(DB30-15:140H)INT value is 320;

}



	/*-------------------------------------200MHz---------------------------------------------
	Reference frequency: 20MHz;Output frequency: 200MHz;VCO frequency: 3200MHz;Prescaler: 8/9;
	RF divider: 16;VCO channel spacing frequency: 200KHz;PFD frequency: 10MHz;
	INT: 320;FRAC: 0;MOD: 100;R: 1;Lock Clk Div: 6; bank clk div: 200; Phase: 1
	----------------------------------------------------------------------------------------*/
	/*RFout = [INT + (FRAC/MOD)] * (Fpfd/RF Divider)*/
	/*Fpfd = REFIN * [(1 + D)/(R * (1 + T))]*/
	/*Fvco = RF divider * Output frequency   2.2G-4.4G*/

void ADF_SetFre(void){

	u32 adf_data;
	u16 adf_R=1;		//RF�ο���Ƶϵ��
  u8 adf_D=0;		//RF REFin��Ƶ��λ(0 or 1)
  u8 adf_T=0;		//�ο�����Ƶλ,����ռ�ձ�50%,��������
  u16 adf_Locktime=160;
  u16 adf_MOD=6;
  u16 adf_INT=320;
  u16 adf_FARC=0;
  u16 adf_PHASE=1;
	u16 adf_RFDiv=RF_div16;
  u8 pinduan;

	CLR_SCL();
	CLR_LE();


	//���üĴ���5
	adf_data = 0x00580000;	//��������   LD���ŵĹ�����ʽΪ��������   D23 D22=01
	adf_data =adf_data | ADF_R5;
	ADF4351_Write(adf_data);


	//���üĴ���4
	adf_data = 0x00800038;
	/*(DB23=1)The signal is taken from the VCO directly,�ź�ֱ�Ӵ�VCO����
	���޸�RF divider, R��ֵ(DB22-20)the RF divider is 16;
	(DB11=0)VCO powerd up;	����RF��������; Ƶ��ѡ��ʱ��,��Ƶ��125k, ��Ƶֵ160*/
	adf_data = adf_data | (adf_RFDiv << 20);		//RF divider is 16
	adf_data = adf_data | (160 << 12);		//Ƶ��ѡ��ʱ��
	adf_data = adf_data | ADF_R4;	//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5dBm
	ADF4351_Write(adf_data);


	//���üĴ���3
	adf_data = 0x00848000;
	/*ѡ����Ƶ�Σ�D23=1��, APB6ns(D22=0,=����С����Ƶʹ��),,(D21=0,С����Ƶʹ��) ʹ��CSR(D18=1),(D16 D15=01)��������
	���޸�clock divider value��ֵ*/
	adf_data = adf_data | (adf_Locktime << 3);
	adf_data = adf_data | ADF_R3;
	ADF4351_Write(adf_data);
	//���üĴ���2
	adf_data = 0x61002040;
	//����ɢ����, ���òο���Ƶ, ����Ƶ������ʹ��(������������)
	//ʹ��˫������, ���õ��ɰ�����0.31, С��N��Ƶ(40), ����R��Ƶ����ֵΪ1
	//���ü������ļ���, (DB6)ͬ���˲���1,�����˲���0,������ͬ���˲���
	adf_data = adf_data | (adf_D << 25);
	adf_data = adf_data | (adf_T << 24);
	adf_data = adf_data | (adf_R << 14);
	adf_data = adf_data | ADF_R2;
	ADF4351_Write(adf_data);

	//���üĴ���1
	adf_data = 0x01008000;
	//������λ����,Ԥ��Ƶ����ֵΪ8/9
  //��λ��Ϊ1
	adf_data = adf_data | (adf_PHASE << 15);
	adf_data = adf_data | (adf_MOD << 3);
	adf_data = adf_data | ADF_R1;
	ADF4351_Write(adf_data);

	//���üĴ���0
	adf_data = 0x00000000;
	adf_data = adf_data | (adf_INT << 15);
	adf_data = adf_data | (adf_FARC << 3);
	adf_data= adf_data | ADF_R0;
	ADF4351_Write(adf_data);

}














//
// /**********************************************************
//                        �����Ƽ�
// ���������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
// https://kvdz.taobao.com/
// **********************************************************/
//
// #include "ADF4351.h"
// #include "stm32f4xx.h"
//
// void delay_us(uint32_t u)
// {
// 	for(int i=u;i--;i>0)
// 	{
// 		for(int q=168;q--;q>0);
// 	}
// }
//
// void ADF_Output_GPIOInit(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;
//
// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
// }
//
// void delay (int length)
// {
// 	while (length >0)
//     	length--;
// }
//
// void WriteToADF4351(uint8_t count, uint8_t *buf)
// {
// 	uint8_t ValueToWrite = 0;
// 	uint8_t i = 0;
// 	uint8_t j = 0;
//
// //	ADF_Output_GPIOInit();
//
// 	ADF4351_CE_SET;
// 	delay_us(1);
// 	ADF4351_CLK_RESET;
// 	ADF4351_LE_RESET;
// 	delay_us(1);
//
// 	for(i = count; i>0; i--)
// 	{
// 		ValueToWrite = *(buf+i-1);
// 		for(j=0; j<8; j++)
// 		{
// 			if(0x80 == (ValueToWrite & 0x80))
// 			{
// 				ADF4351_OUTPUT_DATA_SET;
// 			}
// 			else
// 			{
// 				ADF4351_OUTPUT_DATA_RESET;
// 			}
// 			delay_us(1);
// 			ADF4351_CLK_SET;
// 			delay_us(1);
// 			ValueToWrite <<= 1;
// 			ADF4351_CLK_RESET;
// 		}
// 	}
// 	ADF4351_OUTPUT_DATA_RESET;
// 	delay_us(1);
// 	ADF4351_LE_SET;
// 	delay_us(1);
// 	ADF4351_LE_RESET;
// }
//
// void ADF4351Init(void)
// {
// 	uint8_t buf[4] = {0,0,0,0};
//
// 	ADF_Output_GPIOInit();
//
// 	buf[3] = 0x00;				//�˴����ù̶�����35M
// 	buf[2] = 0x58;
// 	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress
//  	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
// 	WriteToADF4351(4,buf);
//
// 	buf[3] = 0x00;
// 	buf[2] = 0x9c;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 2;(DB19-12:50H)R is 80
// 	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
//  	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
// 	WriteToADF4351(4,buf);
//
// 	buf[3] = 0x00;
// 	buf[2] = 0x00;
// 	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
//  	buf[0] = 0xB3;
// 	WriteToADF4351(4,buf);
//
// 	buf[3] = 0x00;
// 	buf[2] = 0x01;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
// 	buf[1] = 0x4E;				//(DB8=0)enable fractional-N digital lock detect;
//  	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
// 	WriteToADF4351(4,buf);		//(DB23-14:1H)R counter is 1
//
// 	buf[3] = 0x00;
// 	buf[2] = 0x00;
// 	buf[1] = 0x81;			   //(DB14-3:6H)MOD counter is 50;
//  	buf[0] = 0x91;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync
// 	WriteToADF4351(4,buf);	   //nor the spurious optimization functions are being used
// 							   //(DB27=1)prescaler value is 8/9
//
// 	buf[3] = 0x00;
// 	buf[2] = 0x4e;
// 	buf[1] = 0x80;
//  	buf[0] = 0xdb;				//(DB14-3:0H)FRAC value is 27;
// 	WriteToADF4351(4,buf);		//(DB30-15:140H)INT value is 157;
// }
