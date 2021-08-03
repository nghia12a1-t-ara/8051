#include <REGX51.H>
#include <stdio.h>

void Delay_1ms(void);

int main()
{
    int i = 0;
    TMOD = 0x01;
    P2_0 = 0;
    while(1)
    {
        P2_0 = (~P2_0);
        for(i = 0; i<1000; i++)
        {
            Delay_1ms();
        }
    }
}
void Delay_1ms(void)
{
    TH0 = 0xFC;
    TL0 = 0x18;
    TR0 = 1;
    while(!TF0);
    TF0 = 0;
}
