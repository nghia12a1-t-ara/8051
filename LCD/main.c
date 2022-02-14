#include "REG52.h"

sbit RS = P2^0;
sbit EN = P2^1;
#define Data P3

void main()
{
	LCD_Init();
	while(1)
	{
		LCD_cmd(0x01);
	}
}
