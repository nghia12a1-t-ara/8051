/*
 * LCD_8_bit.c
 *
 * http://www.electronicwings.com
 */

#include <reg52.h>
#include <intrins.h>
#include "LCD_8_BIT.h"

#define LCD_Data P2

sbit RS=P3^0;
sbit RW=P3^1;
sbit EN=P3^2;

void delay(unsigned int k)
{
	int i,j;
	for (i=0;i<k;i++)
		for (j=0;j<112;j++);
}

void LCD_Command(char Command)
{
	LCD_Data = Command;
	RS=0;
	RW=0;
	EN=1;
	delay(1);
	EN=0;
	delay(3);
}
void LCD_Char(char Data)
{
	LCD_Data = Data;
	RS=1;
	RW=0;
	EN=1;
	delay(1);
	EN=0;
	delay(3);
}

void LCD_String(unsigned char *str)
{
	int i;
	for(i=0;str[i]!=0;i++)							/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);							/* Call LCD data write */
	}
}

void LCD_String_xy (unsigned char row, unsigned char pos, unsigned char *str)	/* Send string to LCD function */
{
	if (row == 1)
		LCD_Command((pos & 0x0F)|0x80);				/* Command of first row and required position<16 */
	else if (row == 2)
		LCD_Command((pos & 0x0F)|0xC0);				/* Command of Second row and required position<16 */
	LCD_String(str);								/* Call LCD string function */
}

void LCD_Init()
{
	delay(50);
	LCD_Command(0x38);
	LCD_Command(0x0C);
	LCD_Command(0x01);
	LCD_Command(0x06);
	LCD_Command(0x80);
}

