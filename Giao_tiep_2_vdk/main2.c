#include "main.h"
#include "..\Lib\UARTMode1.h"
#include "..\Lib\delay.h"

void main()
{
		UART_Init();
	
	P1 = 0x00;
		
	while(1)
	{
		if(UART_Data_Ready()==1)
		{
			P1 = UART_Read();
		}
	}
}
