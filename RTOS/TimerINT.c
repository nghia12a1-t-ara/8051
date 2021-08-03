#include "REGX52.h"

//void UART_Init(void)
//{
//	// Baudrate = 9600
//	TMOD = 0x20;		// Timer 1
//	TH1 = TL1 = 0xFD;		// Baud = 9600
//	
//	// Frame		//SCON = 0b0101.0000 = 0x50
//	SM0 = 0;
//	SM1 = 1;		// Mode 1, 8 bit data
//	REN = 1;		// Enable Serial	
//	
//	TR1 = 1;  	// Start Timer
//}

//void UART_Transmit(char c)
//{	
//	SBUF = c;				
//	while (TI == 0);		// wait TI = 1 
//	TI = 0;							// Clear trans flag
//}

//char UART_Receive(void)
//{
//	char rei; 
//	while (RI == 0);			// wait RI = 1
//	RI = 0;
//	rei = SBUF;
//	return rei;
//}

//void UART_String(char* str)
//{	
//	int i = 0;
//	while(str[i] != '\0')
//		UART_Transmit(str[i++]);
//}

//void main(void)
//{
//	/* Setup */
//	UART_Init();
//	UART_String("abc");
//	UART_Transmit('A');
//	while(1)
//	{
//		if(UART_Receive() == 'a')
//		{
//			UART_Transmit('b');
//		}
//	}
//}




































//sbit LED = P1^0;
//unsigned int T, Ton, Toff;

//void main()
//{
//		/* Setup Timer Interrupt */
//		TMOD = 0x01;
//	
//		/* INT Enable */
//		ET0 = 1;		// Timer 0 Int Enable
//		EA = 1;			// Global Interrupt Enable
//		
//		/* PWM Parameters */ 
//		T = 1000;
//		Ton = 700;
//		Toff = T - Ton;
//		
//		/* Initialize */ 
//		LED = 1;
//		TH0 = (65535 - Ton)>>8;
//		TL0 = (65535 - Ton)&0x00FF;
//		
//		/* Start Timer */
//		TR0 = 1;
//	
//		while(1)
//		{
//			
//		}
//}

//void TimerOverflow(void) interrupt 1
//{
//	TR0 = 0;
//	LED = ~LED;
//	if(LED == 0)
//	{
//		TH0 = 0xFE;
//		TL0 = 0xD3;
//	}
//	else 		// LED ON
//	{
//		TH0 = 0xFD;
//		TL0 = 0x43;
//	}
//	TR0 = 1;
//}


