/*************************THU VIEN IC 74595***************************
Yeu cau: Khai bao ket noi voi 74595	trong file port.h voi cu phap sau:
sbit IE74595_SHCP = P2^0;
sbit IE74595_DS   = P2^1;
sbit IE74595_STCP = P2^2;
**********************************************************************/

#ifndef _IE74595_H_
#define _IE74595_H_

void IE74595_Shift_Byte(unsigned char b);
void IE74595_Out(unsigned char * bs);

#endif