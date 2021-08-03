#include <REGX51.h>
#include <RTX51TNY.h>

sbit LED_DO = P1^0;
sbit LED_XANH = P1^1;

void Startup(void) _task_ 0
{
	TMOD = 0x21;
	os_create_task (1);
	os_create_task (2);
	os_create_task (0);
}

void Task_LED_DO(void) _task_ 1
{
	while(1)
	{
		LED_DO = 1;
		os_wait (K_TMO, 70, 0);
		LED_DO = 0;
		os_wait (K_TMO, 70, 0);
	}	
}

void Task_LED_XANH(void) _task_ 2
{
	while(1)
	{
		LED_XANH = 1;
		os_wait (K_TMO, 30, 0);
		LED_XANH = 0;
		os_wait (K_TMO, 30, 0);
	}	
}

	