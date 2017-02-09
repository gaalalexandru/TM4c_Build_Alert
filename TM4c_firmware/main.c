/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

/*------------------Project Includes-----------------*/
#include "timer.h"
#include "led.h"
#include "display.h"
#include "sensor.h"
#include "switch.h"
extern uint32_t Baud_Rate_Read;

int main(void)
{
	/* Step1 - Initialize clock and a timer to toggle LED, Init GPIO for RED LED*/
	unsigned long delay = 600000;
	unsigned long ui32SysClock;
	IntMasterDisable();	//Global interrupt disable
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //80 Mhz
	ui32SysClock = SysCtlClockGet();
	//RGB_Led_Init();
	TIMER_Wide_0_Init();

	
	/* Step2 - Initialize ADC and UART for sensor and display*/
	
	Display_Init();
	Sensor_Init();
	Display_NewLine();  
	Display_NewLine();
	Display_String("Clock: ");
	Display_Decimal(ui32SysClock/1000000);
	Display_String(" MHz");
	Display_NewLine();
	Display_String("Baud Rate: ");
	Display_Decimal(Baud_Rate_Read);
	Display_String(" bit/sec");
	Display_NewLine();
	Display_NewLine();
	
	
	/* Step3 - Initialize PWM and GPIO switches to change RGB LED color*/
	/* Step3 - delete RGB_Led_Init() function call from above*/
	
	RGB_PWM_Init(1000);
	TIMER_1_Init();
	TIMER_2_Init();
	Switch_Init();

	IntMasterEnable();	//Global interrupt enable

	/*Step1 - Enter while loop only if clock initialized correctly*/
	while(ui32SysClock)  //Clock working :)
	//while(1)
	{
		//run forever
		/*
		RGB_Led_toggle(BLUE);
		
		while (delay) {
			delay--;
		}
		delay = 600000;
		*/
	}
	return 0;
}
//EOF
