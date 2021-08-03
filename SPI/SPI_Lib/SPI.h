#ifndef __SPI_H__
#define __SPI_H__

#include "REGX51.h"

sbit SPI_MOSI = P2^0;
sbit SPI_MISO = P2^1;
sbit SPI_CLK = P2^2;
sbit SPI_SS = P2^3;

/* Function Prototypes */
void SPI_Master_Init(void);
void SPI_Send(char senddata);
char SPI_Receive(void);

/* include guard */
#endif
