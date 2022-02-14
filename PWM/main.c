#include "regx52.h"

sbit PWM_Pin = P2^0;

unsigned int T, Ton, Toff, Ton_H_reload, Ton_L_reload, Toff_H_reload, Toff_L_reload;

void PWM_Init(unsigned int chuky)
{
	PWM_Pin = 1;
	TMOD &= 0xF0;
	TMOD |= 0x01;		//Timer 0 mode 1
	
	ET0 = 1;				//cho phep ngat timer0
	EA = 1; 				//cho phep ngat toan cuc
	T = chuky; 
}
 void PWM_Start()
{
	TR0 = 1;
}
void PWM_Stop()
{
	TR0 = 0;
}

void PWM_Set_Duty(unsigned char duty)
{
	if(duty == 0)
	{
		PWM_Pin = 0;
		ET0 = 0;
	}
	else if(duty == 100)
	{
		PWM_Pin = 1;
		ET0 = 0;
	}
	else
	{
		Ton = ((unsigned long)T)*duty/100;
		Toff = T - Ton;
	
		Ton_H_reload = (65536 - Ton) >> 8;
		Ton_L_reload = (65536 - Ton) & 0x00FF;
	
		Toff_H_reload = (65536 - Toff) >> 8;
		Toff_L_reload = (65536 - Toff) & 0x00FF;
	
		TH0 = Ton_H_reload;
		TL0 = Ton_L_reload;
	}
}

void main()
{
	PWM_Init(1000); 		//T = 1000us = 1ms
	PWM_Set_Duty(100);
	PWM_Start();
	
	while(1)
	{
	}
}

void Timer0Overflow() interrupt 1
{
	PWM_Pin = !PWM_Pin;
	if(!PWM_Pin)
	{
		TH0 = Toff_H_reload;
		TL0 = Toff_L_reload;
	}
	else
	{
		TH0 = Ton_H_reload;
		TL0 = Ton_L_reload;
	}
}
