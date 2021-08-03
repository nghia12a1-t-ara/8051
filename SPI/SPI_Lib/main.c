#include "REGX51.h"
#include "SPI.h"

int main()
{
	/* MCP4921 4 Control Bits */
	char controlReg;
	unsigned char myData;		/* MCP4921 8 data bits */
	unsigned int DACOutput = 4095;		/* 12 bits data */
	
	/* 0x0789, 0x0789 >> 8 = 0x07, 0x0789 & 0x00FF = 0x0089*/
	
	SPI_Master_Init();
	
	/* Config Control Register: GA = 0 (x2)*/
	controlReg |= (1 << 6) | (1 << 4);	/* Set bit buffer/SHDN */	
	controlReg &= ~(1 << 7);						/* Clear 7th bit, select DACA*/
	
	/* 12 data bits */
	controlReg += (DACOutput >> 8);
	myData = DACOutput & 0x00FF;
	
	/* Send 2 Register Control & data --> MCP4921 */ 
	SPI_SS = 0;
	SPI_Send(controlReg);
	SPI_Send(myData);
	SPI_SS = 1;
	
	while(1)
	{
	}
}

