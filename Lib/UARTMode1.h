#ifndef	_UARTMode1_h_
#define _UARTMode1_h_

void UART_Init();
void UART_Write(char c);
void UART_Write_Text(char *str);
bit UART_Data_Ready();
char UART_Read();

#endif