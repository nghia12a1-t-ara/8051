#include "main.h"
#include "port.h"
#include "..\lib\delay.h"
#include "..\lib\LCD4.h"

unsigned char ADC0804_read()
{
	unsigned char kq;
	ADC0804_CS = 0;
	
	ADC0804_WR = 0;
	ADC0804_WR = 1;
	
	while(ADC0804_INTR);
	
	ADC0804_RD = 0;
	kq = ADC0804_DATA;
	ADC0804_RD = 1;
	
	return kq;
}

void main()
{
	unsigned char adc_value;
	unsigned int voltage;
	
	Lcd_Init();
	
	while(1)
	{
		adc_value = ADC0804_read();
		voltage = adc_value * 19.61f;
		
		Lcd_Chr(1,1,adc_value/100 + 0x30);
		Lcd_Chr_Cp(adc_value%100/10 + 0x30);
		Lcd_Chr_Cp(adc_value%10 + 0x30);

		Lcd_Chr(2,1,voltage/1000 + 0x30);
		Lcd_Chr_Cp('.');
		Lcd_Chr_Cp(voltage%1000/100 + 0x30);
		Lcd_Chr_Cp(voltage%100/10 + 0x30);
		Lcd_Chr_Cp(voltage%10 + 0x30);
		
		Delay_ms(500);
	}
}
		