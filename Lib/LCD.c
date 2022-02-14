#include "LCD.h"
#include "delay.h"
#include "regx51.h"

sbit RS = P2^0;
sbit EN = P2^1;
#define Data P3

// Ham gui 1 lenh ra LCD
void LCD_cmd(unsigned char cmd) 
{
	RS = 0;
	Data = cmd; 
	EN = 0;
	EN = 1;
	delay_ms(2);
}

// Ham khoi tao LCD
void LCD_Init()
{
	LCD_cmd(0x30);
	delay_ms(5);
	LCD_cmd(0x30);
	delay_ms(1);
	LCD_cmd(0x30);
	LCD_cmd(0x38);		//2 dong, font 5x8
	
	LCD_cmd(0x01);		//Xoa noi dung hien thi
	LCD_cmd(0x0C);		//Bat hien thi, tat con tro	
}

// Ham gui du lieu (1 ky tu) ra LCD
void LCD_Chr_cp(char c)
{
	RS = 1;
	Data = c;
	EN = 0;
	EN = 1;
	delay_ms(1);
}

// Ham gui du lieu (1 chuoi) ra LCD
void LCD_out_cp(char *str)
{
	unsigned char i = 0; 
	while(str[i]!=0)
	{
		LCD_chr_cp(str[i]);
		i++;
	}
}

// Ham gui du lieu toi 1 vi tri cu the
void LCD_chr(unsigned char row, unsigned char col, char c)
{
	unsigned char cmd;
	cmd = (row == 1 ? 0x80:0xC0 + col - 1);
	LCD_cmd(cmd);
	LCD_chr_cp(c);
}

// Ham gui du lieu (chuoi) toi 1 vi tri cu the
void LCD_out(unsigned char row, unsigned char col, char *str)
{
	unsigned char cmd;
	cmd = (row == 1 ? 0x80:0xC0) + col - 1;
	LCD_cmd(cmd);
	LCD_out_cp(str);
}

