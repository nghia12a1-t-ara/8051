#include "main.h"
#include "..\lib\delay.h"

void main()
{
	unsigned char i, b;
	while(1)
	{
		while(P3_2);  //chan nhan tin hieu P3.2 = 0, => co tin hieu gui ve
		b = 0;				
		for(i=0; i<8; i++)
		{
			b <<= 1;
			delay_ms(2);
			if(P3_2)			// p3.2 = 1
			{ 
				b |= 0x01;
			}
			else
			{
				while(!P3_2);	// doi P3.2 len 1  (nhan xong bit)
			}
			if(i!=7) while(P3_2); // doi P3.2 xuong 0 de doc tiep
				//bit cuoi cung thi k can doi nua
		}
		P2 = b;
	}
}
