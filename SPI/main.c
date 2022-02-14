#include "main.h"
#include "..\lib\delay.h"

sbit SCLK = P2^0;
sbit MOSI = P2^1;
sbit MISO = P2^2;
sbit CS = P2^3;

void Soft_SPI_Init()
{
	CS = 1;
	SCLK = 0;
	MISO = 1;
}

unsigned char Soft_SPI_Read(void)
{
	unsigned char i, b;
	for(i = 0; i < 8; i++)
	{
		b <<= 1;
		SCLK = 1;
		if(MISO)
			b |= 0x01;
		SCLK = 0;
	}
	return b;
}

void Soft_SPI_Write(unsigned char b)
{ 
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if(b&0x80 != 0)
			MOSI = 1;
		else 
			MOSI = 0;
		SCLK = 1;
		b <<= 1;
		SCLK = 0;
	}
}

void EEPROM_25LCxxx_Write(unsigned int addr, unsigned char b)
{
	CS = 0;
	Soft_SPI_Write(0x06);						//gui lenh WREN
	CS = 1;
	
	CS = 0;
	Soft_SPI_Write(0x02);						//gui lenh WRITE
	Soft_SPI_Write(addr>>8);				//gui byte cao cua dia chi
	Soft_SPI_Write(addr & 0x00FF);	//gui byte thap cua dia chi
	Soft_SPI_Write(b);
	
	CS = 1;
}

void EEPROM_25LCxxx_Read(unsigned int addr)
{
	unsigned char b;
	
	CS = 0;
	SPI_Write(0x03);
	SPI_Write(addr>>8);
	SPI_Write(addr & 0x00FF);
	b = SPI_Read();
	CS = 1;
	
	return b;
}

void main()
{
	Soft_SPI_Init();
	EEPROM_25LCxxx_Write(0,0x55);
	while(1)
	{
	}
}
		