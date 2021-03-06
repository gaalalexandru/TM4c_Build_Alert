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
	int32_t command = 0;
	uint32_t ui32SysClock;
	
	/* Step1 - Initialize clock and a timer to toggle LED, Init GPIO for RED LED*/
	//SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //80 Mhz
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_OSC_INT | SYSCTL_USE_OSC);  //16 Mhz
	ui32SysClock = SysCtlClockGet();
	
	/* Step2 - Initialize UART for receiving commands and display debug info*/
	UART0_Init();
	
	/* Step3 - Initialize GPIO output for PF1,2,3 */
	GPIO_PortF_Init();

	/*Step1 - Enter while loop only if clock initialized correctly*/
	while(ui32SysClock)  //Clock working :)
	{
		//run forever
		command = UART0_In_Char();
		switch (command){
			case 0x00:  //Unchanged status
				GPIO_PortF_Toggle(WHITE);
				break;
			case 0x01:  //Failed build
				GPIO_PortF_Toggle(RED);
				break;
			case 0x02:  //Unstable build
				GPIO_PortF_Toggle(PINK);
				break;
			case 0x03:  //Successfull build
				GPIO_PortF_Toggle(GREEN);
				break;
			default:  //Other message
				GPIO_PortF_Toggle(BLUE);
				}
			TIMER_Wide_0_Init();
	}
	return 0;
}
//EOF
