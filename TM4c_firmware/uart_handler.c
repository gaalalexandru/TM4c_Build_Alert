/*------------------Project Includes-----------------*/
#include "uart_handler.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"

uint32_t Baud_Rate_Read = 0;
/*Step2 - Init UART and write functions to display character, strings, decimal and hex numbers*/

void UART0_Init(void){
	uint32_t uart_config_read = 0;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);  //Enable clock on UART0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);  //Enable clock on port A  //PA0 and PA1 is hard wired to debugger and USB
	UARTDisable(UART0_BASE);  //Disable UART0 while configuration
	
	GPIOPinConfigure(GPIO_PA0_U0RX); //AleGaa PA0
	//GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0); //AleGaa PA0 for RX
	//GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);  //Set the clock source for UART0
	//Next: Set the BAUD RATE, and configure UART0 with 8 data bits, 1 stop bit and no parity bit
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), /*BAUD_RATE*/ 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));  
	UARTFIFOEnable(UART0_BASE);  //Enable the UART FIFO
	UARTEnable(UART0_BASE);  //Enable UART0
	UARTConfigGetExpClk(UART0_BASE, SysCtlClockGet(), &Baud_Rate_Read, &uart_config_read);  //Get the Baud Rate
}

void UART0_Out_Char(unsigned char data){
	UARTCharPut(UART0_BASE, data);
}

void UART0_Out_String(char *pt){
	while(*pt) { //While not the end of the string
		UART0_Out_Char(*pt);  //display current character
		pt++;  //move to next character
	}
}

void UART0_Out_Decimal(unsigned long n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
	if(n >= 10){
		UART0_Out_Decimal(n/10);
		n = n%10;
  }
  UART0_Out_Char(n+'0'); 
	// n is between 0 and 9
}

void UART0_Out_Hex(unsigned long number){
// This function uses recursion to convert the number of
//   unspecified length as an ASCII string
  if(number >= 0x10){
    UART0_Out_Hex(number/0x10);
    UART0_Out_Hex(number%0x10);
  }
  else{
    if(number < 0xA){
      UART0_Out_Char(number+'0');
     }
    else{
      UART0_Out_Char((number-0x0A)+'A');
    }
  }
}

void UART0_Out_NewLine(void){
  UART0_Out_Char(CR);
  UART0_Out_Char(LF);
}

uint32_t UART0_In_Char(void){
	while(!UARTCharsAvail(UART0_BASE));
	return (/*(uint8_t)*/UARTCharGetNonBlocking(UART0_BASE));
}

//EOF
