#include "main.h"
#include "..\Lib\UARTMode1.h"


void main()
{ 
	char i;
	//UART 0 mode 1, Baud=9600
	
	UART_Init();
	
	while(1)
	{
		UART_Write_Text("Alo");
		while(UART_Data_Ready()==0)
		{	
			i = UART_Read();
			UART_Write(i);
		}
	}
}		