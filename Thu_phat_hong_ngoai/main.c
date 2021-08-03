#include "main.h"
#include "..\lib\delay.h"

void Guibit(bit b)
{
	if(b)
	{
		TR2 = 1; 
		delay_ms(1);
		TR2 = 0;
		P1_0 = 1;
		delay_ms(3);
	}
	else
	{
		TR2 = 1;
		delay_ms(3);
		TR2 = 0;
		P1_0 = 1;
		delay_ms(1);
	}
}
void Guibyte(unsigned char b)
{
	unsigned char i; 
	for(i=0; i<8; i++)
	{
		Guibit(b&0x80);
		b <<= 1;
	}
}

void main()
{
	unsigned char cnt = 0;
	
	//khoi tao Timer 2 phat xung 38kHz
	T2CON = 0x00;
	T2MOD = 0x02;
	
	RCAP2H = 0xFF;
	RCAP2L = 0xB1;
	TH2 = 0xFF;
	TL2 = 0xB1;
	
	Guibyte(0x01);
	while(1)
	{
		Guibyte(cnt);
		cnt++;
		delay_ms(10);
	}
}
	