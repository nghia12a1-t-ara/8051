#ifndef _port0808_h_
#define _port0808_h_

//ket noi LCD
sbit LCD_RS = P0^0;
sbit LCD_EN = P0^1;
sbit LCD_D4 = P0^4;
sbit LCD_D5 = P0^5;
sbit LCD_D6 = P0^6;
sbit LCD_D7 = P0^7;

//ket noi ADC
#define ADC_DATA P3;
sbit ADC_ADDA = P2^0;
sbit ADC_ADDB = P2^1;
sbit ADC_ADDC = P2^2;
sbit ADC_ALE = P2^3;
sbit ADC_START = P2^4;
sbit ADC_EOC = P2^5;
sbit ADC_OE = P2^6;

#endif