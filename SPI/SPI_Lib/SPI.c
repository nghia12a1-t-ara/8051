#include "SPI.h"

/* Initialize SPI Master Mode */
void SPI_Master_Init(void)
{
	SPI_SS = 1;
	SPI_CLK = 0;	/* CPOL = 0 */
}

/* Send a byte */
void SPI_Send(char senddata)
{
	int i;
	for(i = 0; i < 8; i++)
	{
		/* LSB First */
		if ( (senddata & 0x80) == 0 )
		{
			SPI_MOSI = 0;
		}
		else  	/* (senddata & 0x80) != 0 */
		{
			SPI_MOSI = 1;
		}
		/* Send a bit by Falling Edge */
		SPI_CLK = 1;			/* CPHA = 0 */
		SPI_CLK = 0;
		senddata <<= 1; 
	}
}

char SPI_Receive(void)
{
	char rdata; /* 0x00 */
	int i;
	for(i = 0; i < 8; i++)
	{
		rdata <<= 1;		// 0x00
		/* Receive a bit by Falling Edge */
		SPI_CLK = 1;			/* CPHA = 0 */
		
		if (SPI_MISO == 1)
		{
			rdata |= 0x01;
		}
		
		SPI_CLK = 0;
	}
	return rdata;
}
