/*
	4x4 Keypad Interfacing with 8051(AT89s52)
	http://www.electronicwings.com
*/


#include<reg52.h>
#include<stdio.h>
#include<string.h>
#include "LCD_8_bit.h"		/* 16x2 LCD header file*/

#define keyport P1

sbit RS 		= P3^5;				/* RS(register select) for LCD16x2 */
sbit RW 		= P3^6;				/* RW(Read/write) for LCD16x2 */
sbit ENABLE = P3^7;				/* EN(Enable) pin for LCD16x2*/

unsigned char keypad[4][4] = {{'7','8','9','/'},
															{'4','5','6','x'},
															{'1','2','3','-'},
															{' ','0','=','+'} };

unsigned char colloc, rowloc;		
											
															
unsigned char key_detect()
{
		/* F <=> Input pullup <=> COL, 0 <=> OUTPUT <=> ROW */
		keyport=0xF0;							
		/*set port direction as input-output*/
		do
		{
			keyport = 0xF0;				
			colloc = keyport;
			colloc &= 0xF0;				/* mask port for column read only */
		}while(colloc != 0xF0);		/* read status of column */
	
			do
			{
				do
				{
					delay(20);									/* 20ms key debounce time */
					colloc = (keyport & 0xF0);	/* read status of column */
				}while(colloc == 0xF0);				/* check for any key press */
		
				delay(1);
				colloc = (keyport & 0xF0);
			}while(colloc == 0xF0);
			
	/* Key detect - Row & Col */
	while(1)
	{
		/* now check for rows */
			keyport= 0xFE;					/* 0b1111.1110 */					/* check for pressed key in 1st row */
			colloc = (keyport & 0xF0);		/* 0b1011.0000 */		/* Colloc = 0xB0 */
			if(colloc != 0xF0)			/* check button click */
			{
				rowloc = 0;				/* clicked button is in 1st row */
				break;
			}

			keyport = 0xFD;				/* 0b1111.1101 */					/* check for pressed key in 2nd row */
			colloc = (keyport & 0xF0);	/* 0b1111.0000 */		/* Colloc = 0xF0 */
			if(colloc != 0xF0)	/* check button click */
			{
				rowloc = 1;				/* clicked button is in 2nd row */
				break;
			}
			
			keyport = 0xFB;									/* check for pressed key in 3rd row */
			colloc = (keyport & 0xF0);
			if(colloc != 0xF0)
			{
				rowloc = 2;
				break;
			}

			keyport = 0xF7;									/* check for pressed key in 4th row */
			colloc = (keyport & 0xF0);
			if(colloc != 0xF0)
			{
				rowloc = 3;
				break;
			}	
	}
	
		if(colloc == 0xE0)
		{
			return(keypad[rowloc][0]);
		}		
		else if(colloc == 0xD0)
		{
			return(keypad[rowloc][1]);
		}	
		else if(colloc == 0xB0)
		{
			return(keypad[rowloc][2]);
		}	
		else
		{
			return(keypad[rowloc][3]);
		}	
}


int main(void)
{
	/* set - up */
	LCD_Init(); 
	LCD_String_xy(1,0,"Press a key");
	/* Gas sensor Init */
	a = 1;
	/* Loop */
  while(1)			/* Polling */
	{
		LCD_Command(0xc0);
		LCD_Char(key_detect(a));							/* Display which key is pressed */
		/* 10s */
	}		
}

void Interuppt()
{
		a = 0;
		/* Gas Sensor */
		if (Sensor_detect())			// <1ms
		{
			/* Warning!!! */
		}
		
		/* check button */
}
