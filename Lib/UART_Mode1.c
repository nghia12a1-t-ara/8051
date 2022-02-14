#include "main.h"
#include "..\Lib\UARTMode1.h"

void UART_Init()
{
	SM0 = 0; SM1 = 1;  // chon UART mode 1
	TMOD = 0x20;		   // 0010 xxxx chon Timer 1 che do 8 bit tu dong nap lai  
	TH1 = 0xFD;				 // chon toc do Baud = 9600	
	TR1 = 1;					 // cho phep Timer 1
	TI = 1; 					 // san sang gui du lieu
	REN = 1;  				 // cho phep nhan du lieu	
}

void UART_Write(char c) 
{
	while(TI==0);
	TI = 0;
	SBUF = c;
}

void UART_Write_Text(char *str)
{
	unsigned char i = 0; 
	while(str[i] != 0)
	{
		UART_Write(str[i]);
		i++;
	}
}

bit UART_Data_Ready()
{ 
	return RI;
}

char UART_Read()
{
	RI = 0;
	return SBUF;
}
