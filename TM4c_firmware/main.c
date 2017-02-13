/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

/*------------------Project Includes-----------------*/
#include "gpio_handler.h"
#include "uart_handler.h"
#include "timer_handler.h"
/*-------------------Macro Definitions----------------*/

// Color    LED(s) PortF
// DARK     ---    0
// RED      R--    0x02 - 1
// BLUE     --B    0x04 - 2
// GREEN    -G-    0x08 
// YELLOW   RG-    0x0A
// SKY_BLUE -GB    0x0C
// WHITE    RGB    0x0E
// PINK     R-B    0x06

uint8_t timer_flag = 0;
extern uint8_t Baud_Rate_Read;

#define UART_DEBUG

int main(void)
{
	uint8_t command = 0;
	uint8_t old_command = 0;
	unsigned long ui32SysClock;
	
	/* Step1 - Initialize clock and a timer to toggle LED, Init GPIO for RED LED*/
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //80 Mhz
	ui32SysClock = SysCtlClockGet();
	
	/* Step2 - Initialize UART for receiving commands and display debug info*/
	UART0_Init();
	
#ifdef UART_DEBUG
	UART0_Out_NewLine();  
	UART0_Out_String("Clock: ");
	UART0_Out_Decimal(ui32SysClock/1000000);
	UART0_Out_String(" MHz");
	UART0_Out_NewLine();
	UART0_Out_String("Baud Rate: ");
	UART0_Out_Decimal(Baud_Rate_Read);
	UART0_Out_String(" bit/sec");
	UART0_Out_NewLine();
#endif
	
	/* Step3 - Initialize GPIO output for PF1,2,3 */
	GPIO_PortF_Init();
	TIMER_Wide_0_Init();

	/*Step1 - Enter while loop only if clock initialized correctly*/
	while(ui32SysClock)  //Clock working :)
	{
		//run forever
		if(timer_flag ==1){
			command = UART0_In_Char();
	#ifdef UART_DEBUG
		UART0_Out_NewLine();  
		UART0_Out_String("Received command: ");
		UART0_Out_Char(command);
	#endif
			if(old_command != command){
				switch (command){
					case 0:
						GPIO_PortF_Toggle(WHITE);
						break;
					case 1:
						GPIO_PortF_Toggle(RED);
						break;
					case 2:
						GPIO_PortF_Toggle(BLUE);
						break;
					case 3:
						GPIO_PortF_Toggle(GREEN);
					default:
						break;
				}
				old_command  = command;
			}
			timer_flag = 0; //reset flag
		}
	}
	return 0;
}
//EOF
