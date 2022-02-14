/*------------------------------------------------------------------*-
		Soft_I2c.h
	---------------------------------------
		- Written by Nguyen Thanh Dang
		- Created date: 	16/05/2012
		- Last Modified: 	16/05/2012
		- Require: 
			* Declare hardware connections (in Port.h). Ex:
				sbit SOFT_I2C_SCL	= P1^0;
				sbit SOFT_I2C_SDA	= P1^1; 
			* Define USE_SOFT_I2C_DELAY when you want reduce frequency of I2C
			* Use LX51 (Option for Target - Device tab - 
				Use Extended Linker (LX51)...)
			* Use REMOVEUNUSED linker directive (Option for Target - 
				LX51 Misc tab - Misc controls: RU) 
-*------------------------------------------------------------------*/
#ifndef	_SOFT_I2C_H_
#define _SOFT_I2C_H_

// Init
void Soft_I2c_Init();

// Master generate Start signal
void Soft_I2c_Start();

// Master generate Stop signal
void Soft_I2c_Stop();

// Write data to Slaver, and get ACK/NACK from Slaver
bit Soft_I2c_Write(unsigned char dat);

// Read data from Slaver
unsigned char Soft_I2c_Read(bit ack);

#endif
/*------------------------------------------------------------------*-
	--- Ket thuc file ---------------------
-*------------------------------------------------------------------*/
