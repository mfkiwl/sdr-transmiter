#include <iostream>
#include <fstream> 
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

using namespace std;
int main(int argc, char **argv)
{	
	ifstream infile(argv[2]); 
	int ROMunitSize=1;
	int ROMsize=atoi(argv[1]);

	printf("DEPTH = %d;\r\nWIDTH = %d;\r\nADDRESS_RADIX = HEX;\r\nDATA_RADIX = DEC;\r\nCONTENT\r\nBEGIN\r\n",ROMsize,ROMunitSize*8);	
	uint16_t addr=0;
	for(int i=0;i<ROMsize;i++)
	{	
		int tmp;
		char cTemp;
		infile >> tmp >> cTemp;
		printf("%4x : %d;\r\n", addr, tmp);
		addr += ROMunitSize;
	}
	printf("END;");
	return 0;
}
