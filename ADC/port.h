#ifndef _port_h_
#define _port_h_

//ket noi LCD
sbit LCD_RS = P0^0;
sbit LCD_EN = P0^1;
sbit LCD_D4 = P0^4;
sbit LCD_D5 = P0^5;
sbit LCD_D6 = P0^6;
sbit LCD_D7 = P0^7;

//ket not ADC
#define ADC0804_DATA	P2;
sbit ADC0804_CS = P3^0;
sbit ADC0804_RD = P3^1;
sbit ADC0804_WR = P3^2;
sbit ADC0804_INTR = P3^3;

#endif
