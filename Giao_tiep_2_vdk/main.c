#include "main.h"
#include "..\Lib\UARTMode1.h"
#include "..\Lib\delay.h"

void main()
{
	UART_Init();
	
	while(1)
	{
		if(P1_0==0)
		{ 
				delay_ms(20);
				UART_Write(0x01);
				while(P1_0==0);
		}
		if(P1_1==0)
		{
				delay_ms(20);
				UART_Write(0x00);
				while(P1_1==0);
		}
	}
}
