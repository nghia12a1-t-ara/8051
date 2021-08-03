#include "REGX52.H"

void delay_ms(unsigned int delay_time);

void main()
{
	// set up 
	P1_0 = 0;		// LED OFF 
	
	// super loop
	while(1)
	{
		P1_0 != P1_0;
		delay_ms(1000);
	}
}

void delay_ms(unsigned int delay_time)
{
	int i = 0;
	while(delay_time--)
	{
		for (i = 0; i < 100; i++); 
	}
}
