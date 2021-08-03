#include<main.h>
#include<..\lib\delay.h>

void UART_Mode0_Write(unsigned char b)
{
	SBUF = b;
	while(TI == 0);
	TI = 0;
	
	P3_2 = 0;
	P3_2 = 1;
}
void main()
{	
		SM0 = SM1 = 0;
	while (1)
	{
		UART_Mode0_Write(0x00);
		delay_ms(1000);
		UART_Mode0_Write(0xFF);
		delay_ms(1000);
	}
}
