/*
 * LCD_8_bit.h
 *
 * http://www.electronicwings.com
 */

#ifndef __LCD_8_BIT_H__
#define __LCD_8_BIT_H__

#include <reg52.h>

void delay(unsigned int);
void LCD_Command(char Command);
void LCD_Char(char Data);
void LCD_String(unsigned char *ptr);
void LCD_String_xy(unsigned char row,unsigned char column, unsigned char *);
void LCD_Init();

#endif