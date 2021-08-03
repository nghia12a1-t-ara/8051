#include "main.h"
#include "port0808.h"
#include "..\lib\delay.h"
#include "..\lib\LCD4.h"

void ADC0808_Init()
{
	ADC_START = 0;
	ADC_ALE = 0;
	ADC_OE = 0; 
}

unsigned char ADC0808_Read(unsigned char channel)
{
	unsigned char kq;
	
	ADC_ADDA = channel & 0x01;
	ADC_ADDB = channel & 0x02;
	ADC_ADDC = channel & 0x04;
	
	ADC_ALE = 1;
	ADC_START = 1;
	ADC_ALE = 0; 
	ADC_START = 0;
	
	while(ADC_EOC);
	while(!ADC_EOC); 			//doi EOC xuong 0, roi lai len 1 de chuyen doi hoan tat
	
	ADC_OE = 1;
	kq = ADC_DATA;
	ADC_OE = 0;
	
	return kq;
}

void main()
{
	unsigned char adc_value;
	unsigned int voltage;
	
	Lcd_Init();
	ADC0808_Init();
	while(1)
	{
		adc_value = ADC0808_Read(0);
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
