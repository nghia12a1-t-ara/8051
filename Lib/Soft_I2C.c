/*------------------------------------------------------------------*-
		Soft_I2c.c
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

#include"main.h"
#include"port.h"
#include"Soft_I2c.h"
#include"intrins.h"

#ifdef	USE_SOFT_I2C_DELAY
	#message "Soft I2C - USE_SOFT_I2C_DELAY"
	#define Soft_I2c_Delay()	{_nop_();_nop_();_nop_();_nop_();_nop_();}
#else
	#define Soft_I2c_Delay()
#endif

/*-------------------------------------*-
	Prototype for Local Function
-*-------------------------------------*/
bit Soft_I2c_Get_Ack();
void Soft_I2c_Ack();
void Soft_I2c_Nak();

/*-------------------------------------*-
	Soft_I2c_Init
-*-------------------------------------*/
void Soft_I2c_Init()
{
 	SOFT_I2C_SCL=1;
	SOFT_I2C_SDA=1;
}

/*-------------------------------------*-
	Soft_I2c_Start
-*-------------------------------------*/
void Soft_I2c_Start()
{
	SOFT_I2C_SCL = 1;
	Soft_I2c_Delay();
	SOFT_I2C_SDA = 0;
	Soft_I2c_Delay();
	SOFT_I2C_SCL = 0;
}

/*-------------------------------------*-
	Soft_I2c_Get_Ack - Local Function 
	- return
		0 - ACK
		1 - NAK 
-*-------------------------------------*/
bit Soft_I2c_Get_Ack()
{
	bit result;
    SOFT_I2C_SDA = 1;
	Soft_I2c_Delay();
    SOFT_I2C_SCL = 1;
	Soft_I2c_Delay();
	result = SOFT_I2C_SDA;
    SOFT_I2C_SCL = 0;
	return result;
}

/*-------------------------------------*-
	Soft_I2c_Write
	- return
		0: ACK - No Error
		1: NAK - Error
-*-------------------------------------*/
bit Soft_I2c_Write(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
	    SOFT_I2C_SDA = (bit)(dat&0x80);	
	    SOFT_I2C_SCL = 1;
		Soft_I2c_Delay();
		SOFT_I2C_SCL = 0;
	    dat<<=1;
	}
	return(Soft_I2c_Get_Ack());
}

/*-------------------------------------*-
	Soft_I2c_Ack - Local Function
-*-------------------------------------*/
void Soft_I2c_Ack()
{
	SOFT_I2C_SDA = 0;
	Soft_I2c_Delay();
	SOFT_I2C_SCL = 1;
	Soft_I2c_Delay();
    SOFT_I2C_SCL = 0;
}

/*-------------------------------------*-
	I2C_Nak - Local Function	
-*-------------------------------------*/
void Soft_I2c_Nak()
{
    SOFT_I2C_SDA = 1;
	Soft_I2c_Delay();
    SOFT_I2C_SCL = 1;
	Soft_I2c_Delay();
    SOFT_I2C_SCL = 0;
}

/*-------------------------------------*-
	I2C_Read function
	- Tham so
		ack: 0 - Master tao NAK sau khi truyen
			 1 - Master tao ACK sau khi truyen
-*-------------------------------------*/
unsigned char Soft_I2c_Read(bit ack)
{
    unsigned char i, dat=0;
    for(i=0;i<8;i++)
	{
	    SOFT_I2C_SDA = 1;
		Soft_I2c_Delay();
	    SOFT_I2C_SCL = 1;
		Soft_I2c_Delay();
	    dat <<= 1;
		if(SOFT_I2C_SDA)
		{
		 	dat |= 0x01;
		}
		SOFT_I2C_SCL = 0;
    }
	if(ack)
	{
	 	Soft_I2c_Ack();
	}
	else
	{
	 	Soft_I2c_Nak();
	}
    return dat;
}

/*-------------------------------------*-
	Soft_I2c_Stop function
-*-------------------------------------*/
void Soft_I2c_Stop()
{ 	
	SOFT_I2C_SDA = 0;
	Soft_I2c_Delay();
	SOFT_I2C_SCL = 1;
	Soft_I2c_Delay();
	SOFT_I2C_SDA = 1;
}
/*------------------------------------------------------------------*-
	--- Ket thuc file ---------------------
-*------------------------------------------------------------------*/
