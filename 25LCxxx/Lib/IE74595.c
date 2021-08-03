// MO RONG NGO RA DUNG IC 74HC595

// Yeu cau: Tao file port.h chua cac ket noi phan cung nhu sau:
//sbit IE74595_SHCP	= P2^0;
//sbit IE74595_DS 	= P2^1;
//sbit IE74595_STCP 	= P2^2;

#include"main.h"
#include"IE74595.h"
#include"Port.h"

 void IE74595_Out(unsigned char *p, unsigned char n)
{
	unsigned char i, b, j;
	for(j=0;j<n;j++)
	{
		b = *(p+n-j-1);		// Lay byte cao nhat truoc

		for(i=0;i<8;i++)
		{
		 	IE74595_DS = b & (0x80>>i);		// Lay tung Bit cua byte b gui ra DS, lay bit MSB truoc
	
			// Tao xung dich du lieu
			IE74595_SHCP = 0;
			IE74595_SHCP = 1;
		}
	}
	// Tao xung xuat du lieu
	IE74595_STCP = 0;
	IE74595_STCP = 1;
}