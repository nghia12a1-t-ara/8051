#include "main.h"
#include "port0808.h"

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
