#include "main.h"

void delay_ms_T0(unsigned int t)
{
	do {
		TL0 = 0x18;
		TH0 = 0xFC;
		TR0 = 1;
		while(!TF0);
		TR0 = 0;
		TF0 = 0;
		t--;
	}while(t!=0);
}
void main()
{
	TMOD = 0x01;
	while(1)
	{
		P2_0 = !P2_0;
		delay_ms_T0(1000);
	}
}
	