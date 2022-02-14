/*************************THU VIEN IC 74595***************************
Yeu cau: Khai bao ket noi voi 74595	trong file port.h voi cu phap sau:
sbit IE74595_SHCP = P2^0;
sbit IE74595_DS   = P2^1;
sbit IE74595_STCP = P2^2;
**********************************************************************/

#include"main.h"
#include"port.h"
#include"IE74595.h"

void IE74595_Shift_Byte(unsigned char b)
{
 	unsigned char i;
	for(i = 0; i<8; i++)
	{
	 	// Xuat bit 7 den bit 0	ra chan DS
		IE74595_DS = (b >> (7-i))&0x01;

		// Tao xung SHCP
		IE74595_SHCP = 0;
		IE74595_SHCP = 1;
	}

	// Tao xung STCP
	IE74595_STCP = 0;
	IE74595_STCP = 1;
}

// Ta viet them 1 ham xuat 1 mang byte ra cac IC74595
// bs la con tro nha
void IE74595_Out(unsigned char * bs)
{
 	unsigned char i, j;
	unsigned char n;
	unsigned char b;

	n = sizeof(bs);

	for(j=n;j>0;j--)
	{
		// Ta dich byte cao truoc
		b = *(bs+j-1);
		for(i = 0; i<8; i++)
		{
		 	// Xuat bit 7 den bit 0	ra chan DS
			IE74595_DS = (b >> (7-i))&0x01;
	
			// Tao xung SHCP
			IE74595_SHCP = 0;
			IE74595_SHCP = 1;
		}
	}

	// Tao xung STCP
	IE74595_STCP = 0;
	IE74595_STCP = 1;
}


