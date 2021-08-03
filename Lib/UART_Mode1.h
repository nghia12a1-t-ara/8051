#ifndef	_UART_Mode1_h_
#define _UART_Mode1_h_

void UART_Write(char c);
void UART_Write_Text(char *str);
bit UART_Data_Ready();
char UART_Read();

#endif