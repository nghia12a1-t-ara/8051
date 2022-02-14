#ifndef _LCD_H_
#define _LCD_H_

void LCD_cmd(unsigned char cmd);
void LCD_Init();
void LCD_Chr_cp(char c);
void LCD_out_cp(char *str);
void LCD_chr(unsigned char row, unsigned char col, char c);
void LCD_out(unsigned char row, unsigned char col, char *str);

#endif