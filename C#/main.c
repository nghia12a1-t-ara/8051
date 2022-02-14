#include "main.h"
#include "stdio.h"
void main()
{	
	char c;

	SM0 = 0; SM1 = 1;  // chon UART mode 1
	TMOD = 0x20;		   // 0010 xxxx chon Timer 1 che do 8 bit tu dong nap lai  
	TH1 = 0xFD;				 // chon toc do Baud = 9600	
	TR1 = 1;					 // cho phep Timer 1
	TI = 1; 					 // san sang gui du lieu
	REN = 1;  	
	
	P2_0 = 0;
	P2_1 = 0;
	putchar(65);
	while(1)
	{
			c = _getkey();
		switch(c)
		{
			case '0':
					P2_0 = !P2_0;
					if(P2_0)
						printf("LED is ON!\n");
					else
						printf("LED is OFF!\n");
					break; 
			case '1':
					P2_1 = !P2_1;
					if(P2_1)
						printf("MOTOR is ON!\n");
					else
						printf("MOTOR is OFF!\n");
					break; 
		}
	}
}