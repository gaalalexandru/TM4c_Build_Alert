#ifndef DISPLAY
#define DISPLAY

/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-------------------Macro Definitions----------------*/
// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1
// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F
#define BAUD_RATE (115200)

/*-----------External function definisions-----------*/
/*Step2 - Init UART and write functions to display character, strings, decimal and hex numbers*/

void UART0_Init(void);
void UART0_Out_Char(unsigned char data);
uint8_t UART0_In_Char(void);
void UART0_Out_String(char *pt);
void UART0_Out_Decimal(unsigned long n);
void UART0_Out_Hex(unsigned long number);
void UART0_Out_NewLine(void);

#endif
//EOF
