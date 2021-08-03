/*------------------------------------------------------------------*-
		Lcd4.c
	---------------------------------------
		- Written by Nguyen Thanh Dang
		- Created date: 	16/05/2012
		- Last Modified:	16/05/2012

		- Require:	
			* Declare hardware connections (in Port.h). Ex:
				sbit LCD_RS	= P0^0;
				sbit LCD_RW = P0^1; (optional)
				sbit LCD_EN	= P0^2;
				sbit LCD_D4	= P2^4;
				sbit LCD_D5 = P2^5;
				sbit LCD_D6 = P2^6;
				sbit LCD_D7 = P2^7;
			* Define USE_LCD_RW and USE_CHECK_BUSY (in Main.h) if you need. Ex:
				#define USE_LCD_RW
				#define USE_CHECK_BUSY	
			* Use LX51 (Option for Target - Device tab - 
				Use Extended Linker (LX51)...)
			* Use REMOVEUNUSED linker directive (Option for Target - 
				LX51 Misc tab - Misc controls: RU)
-*------------------------------------------------------------------*/
#include"Main.h"
#include"Port.h"
#include"LCD4.h"
#include"Delay.h"
#include"String.h"

#ifdef	USE_CHECK_BUSY
 	#message "Lcd - Use check busy method."
	#ifndef USE_LCD_RW
		#error "Lcd - Define USE_LCD_RW, please."
	#endif
#else
	#message "Lcd - Use delay method."
#endif

/*-------------------------------------*-
	Prototype of Local Functions	
-*-------------------------------------*/
void Lcd_Write_High_Nibble(unsigned char);
void Lcd_Write_Low_Nibble(unsigned char );
void Lcd_Delay_us(unsigned char);
#ifdef	USE_CHECK_BUSY
void Lcd_Busy();
#endif


/*-------------------------------------*-
	Lcd_Write_High_Nibble - Local Function
-*-------------------------------------*/
void Lcd_Write_High_Nibble(unsigned char b)
{
	LCD_D7 = b & 0x80;
	LCD_D6 = b & 0x40;
	LCD_D5 = b & 0x20;
	LCD_D4 = b & 0x10; 	
}

/*-------------------------------------*-
	Lcd_Write_High_Low - Local Function
-*-------------------------------------*/
void Lcd_Write_Low_Nibble(unsigned char b)
{
 	LCD_D7 = b & 0x08;
	LCD_D6 = b & 0x04;
	LCD_D5 = b & 0x02;
	LCD_D4 = b & 0x01;
}

/*-------------------------------------*-
	Lcd_Delay_us - Local Function
-*-------------------------------------*/
void Lcd_Delay_us(unsigned char t)
{
 	while(t--);
}

/*-------------------------------------*-
	Lcd_Busy - Local Function
-*-------------------------------------*/
#ifdef	USE_CHECK_BUSY
void Lcd_Busy()
{
	bit busy_flag;
	LCD_D7 = 1;			// Read data
	LCD_RS = 0;
	LCD_RW = 1;			// Read
	do{
		LCD_EN = 0;		// Read High Nibble
		LCD_EN = 1;
		busy_flag = LCD_D7;
		LCD_EN = 0;	 	// Read Low Nibble
		LCD_EN = 1;
	}while(busy_flag);
	LCD_EN = 0;	
}
#endif

/*-------------------------------------*-
	Lcd_Init
	- Khoi tao LCD che do 4 bit, font 5x7
-*-------------------------------------*/
void Lcd_Init()
{	
	LCD_RS = 0;
	LCD_EN = 0;
#ifdef	USE_LCD_RW
	LCD_RW = 0;
#endif
	
	Delay_ms(20);

	Lcd_Write_Low_Nibble(0x03);
	LCD_EN = 1;
	LCD_EN = 0;
    Delay_ms(5);

	Lcd_Write_Low_Nibble(0x03);
	LCD_EN = 1;
	LCD_EN = 0;
    Lcd_Delay_us(100);

	Lcd_Write_Low_Nibble(0x03);
	LCD_EN = 1;
	LCD_EN = 0;
#ifdef	USE_CHECK_BUSY
	Lcd_Busy(); 
#else
	Delay_ms(1);
#endif

	Lcd_Write_Low_Nibble(0x02);
	LCD_EN = 1;
	LCD_EN = 0;
	Delay_ms(1);
		
	Lcd_Cmd(_LCD_4BIT_2LINE_5x7FONT);
	Lcd_Cmd(_LCD_TURN_ON);
	Lcd_Cmd(_LCD_CLEAR);
	Lcd_Cmd(_LCD_ENTRY_MODE);
}

/*-------------------------------------*-
	Lcd_Cmd
	- Gui lenh cho LCD
-*-------------------------------------*/
void Lcd_Cmd(unsigned char cmd)
{
#ifdef	USE_LCD_RW
	LCD_RW = 0;
#endif
	LCD_RS = 0;
	Lcd_Write_High_Nibble(cmd); 
	LCD_EN = 1;
	LCD_EN = 0;

	Lcd_Write_Low_Nibble(cmd);
	LCD_EN = 1;
	LCD_EN = 0;

#ifdef	USE_CHECK_BUSY
	Lcd_Busy(); 
#else
	switch(cmd)
	{
		case _LCD_CLEAR:
		case _LCD_RETURN_HOME:
			Delay_ms(2);
			break;
		default:
			Lcd_Delay_us(37);
			break;
	}
#endif
}

/*-------------------------------------*-
	Lcd_Chr_Cp
-*-------------------------------------*/
void Lcd_Chr_Cp(unsigned char achar)
{
#ifdef	USE_LCD_RW
	LCD_RW = 0;
#endif
	LCD_RS = 1;
	Lcd_Write_High_Nibble(achar);
	LCD_EN = 1;
	LCD_EN = 0;
	
	Lcd_Write_Low_Nibble(achar);
	LCD_EN = 1;
	LCD_EN = 0;
	
#ifdef	USE_CHECK_BUSY
	Lcd_Busy(); 
#else
	Lcd_Delay_us(37+4);
#endif	
} 

/*-------------------------------------*-
	Lcd_Chr
	- In ky tu ra man hinh tai (row, column)
-*-------------------------------------*/
void Lcd_Chr(unsigned char row, unsigned char column, 
	unsigned char out_char)
{
	unsigned char add;
	add = (row==1?0x80:0xC0);
	add += (column - 1);
	Lcd_Cmd(add);
	Lcd_Chr_Cp(out_char);
}

void Lcd_Out_Cp(unsigned char * str)
{
	unsigned char i = 0;
	while(str[i])
	{
		Lcd_Chr_Cp(str[i]);
	 	i++;
	}
}

/*-------------------------------------*-
	Lcd_Out
	- In chuoi (text) ra man hinh
	- Vi tri bat dau tai (row, column)
-*-------------------------------------*/
void Lcd_Out(unsigned char row, unsigned char column, 
	unsigned char* text)
{
	unsigned char add;
	add = (row==1?0x80:0xC0);
	add += (column - 1);
	Lcd_Cmd(add);
	Lcd_Out_Cp(text); 
}

/*-------------------------------------*-
	Lcd_Custom_Chr Function
	- Tao ky tu tren LCD
	- Tham so:
		location: Vi tri tren CGRAM (0÷7)
		lcd_char: Con tro den mang Font ky tu
	- Ex: Tao ky tu "Clock" sau do in len dong 1, cot 1
		unsigned char code Lcd_Char_Clock[] = {14,21,21,23,17,17,14,0};
		//...
		Lcd_Init();
		Lcd_Custom_Chr(0,Lcd_Char_Clock);
		Lcd_Chr(1,1,0);
		//...
-*-------------------------------------*/
void Lcd_Custom_Chr(unsigned char location, unsigned char * lcd_char) 
{
  	unsigned char i;
    Lcd_Cmd(0x40+location*8);
    for (i = 0; i<=7; i++) 
		Lcd_Chr_Cp(lcd_char[i]);
}

/*------------------------------------------------------------------*-
	--- Ket thuc file ---------------------
-*------------------------------------------------------------------*/