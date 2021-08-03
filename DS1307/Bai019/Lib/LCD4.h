/*------------------------------------------------------------------*-
		Lcd4.h
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
#ifndef _LCD4BIT_H_
#define _LCD4BIT_H_

/*-------------------------------------*-
	LCD commands
-*-------------------------------------*/ 
#define _LCD_CLEAR				1   		
#define _LCD_RETURN_HOME		2
#define _LCD_ENTRY_MODE			6
#define _LCD_TURN_OFF			8
#define _LCD_TURN_ON			12
#define _LCD_CURSOR_OFF			12 	  
#define _LCD_UNDERLINE_ON		14
#define _LCD_BLINK_CURSOR_ON	15
#define _LCD_MOVE_CURSOR_LEFT	16
#define _LCD_MOVE_CURSOR_RIGHT	17
#define _LCD_SHIFT_LEFT			24
#define _LCD_SHIFT_RIGHT		28
#define _LCD_4BIT_1LINE_5x7FONT	0x20
#define _LCD_4BIT_2LINE_5x7FONT	0x28
#define _LCD_8BIT_1LINE_5x7FONT	0x30
#define _LCD_8BIT_2LINE_5x7FONT	0x38
#define _LCD_FIRST_ROW			128
#define _LCD_SECOND_ROW			192
/*-------------------------------------*-
	End of LCD commands
-*-------------------------------------*/

void Lcd_Init(void);

void Lcd_Cmd(unsigned char cmd);

void Lcd_Chr_Cp(unsigned char achar);
	
void Lcd_Chr(unsigned char row, unsigned char columnm,
	unsigned char achar);

void Lcd_Out_Cp(unsigned char * str);

void Lcd_Out(unsigned char row, unsigned char column,
	unsigned char * str);

void Lcd_Custom_Chr(unsigned char location, 
	unsigned char * lcd_char);

#endif
/*------------------------------------------------------------------*-
	--- Ket thuc file ---------------------
-*------------------------------------------------------------------*/