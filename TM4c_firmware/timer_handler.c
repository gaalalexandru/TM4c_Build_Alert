/*------------------Project Includes-----------------*/
#include "timer_handler.h"
//#include "led.h"
//#include "display.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

/*-------------------Macro Definitions----------------*/
#define WTIMER0_PRIO (0x01)
#define TIMER1_PRIO (0x00)
#define TIMER2_PRIO (0x00)

extern uint8_t timer_flag;


/*Step1 - Init Wide Timer0 and write Wide Timer0 ISR*/

unsigned long TIMER_reload_calculator(unsigned long delay_time_ms)
{
	unsigned long clock_cycle_required = 0;
	unsigned long clock_speed = 0;
	
	clock_speed = SysCtlClockGet();	//Get clock speed in Hz
	clock_cycle_required = ((clock_speed / 1000) * delay_time_ms) - 1;	//Returns the clock cycles required for the requested time in mili seconds
	return clock_cycle_required;
}

void TIMER_Wide_0_Init(void)	//cyclic wide timer configuration
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);		//Wide Timer 0 enable 
	IntDisable(INT_WTIMER0A);													//Wide Timer 0A disable of interrupts
	TimerIntDisable(WTIMER0_BASE,TIMER_TIMA_TIMEOUT);
	
	TimerDisable(WTIMER0_BASE, TIMER_A);
	TimerClockSourceSet(WTIMER0_BASE, TIMER_CLOCK_SYSTEM);
	TimerConfigure(WTIMER0_BASE, TIMER_CFG_A_PERIODIC);

	TimerLoadSet(WTIMER0_BASE, TIMER_A, TIMER_reload_calculator(500));	//Set cycle nr for 1000 ms	
	TimerEnable(WTIMER0_BASE, TIMER_A);
	
	TimerIntEnable(WTIMER0_BASE,TIMER_TIMA_TIMEOUT);
	IntPrioritySet(INT_WTIMER0A,(WTIMER0_PRIO)<<5); 			//Priority 1 = "001"0.0000
	IntEnable(INT_WTIMER0A);	//Wide Timer 0A enable of interrupts
}


void WTIMER0A_Handler(void)		//Wide Timer 0 A ISR
{
	//unsigned long timer_value=0;
	if(TimerIntStatus(WTIMER0_BASE,false))
	{
		TimerIntClear(WTIMER0_BASE, TIMER_A);
		//timer_value = TimerValueGet(WTIMER0_BASE, TIMER_A);
		IntMasterDisable();	//Global interrupt disable
		timer_flag = 1;
 	  IntMasterEnable();	//Global interrupt enable
	}
}

void TIMER_1_Init(void)	//cyclic timer configuration for PF0 / SW2 debounce
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);  //Wide Timer 0 enable 
	IntDisable(INT_TIMER1A);  //Wide Timer 0A disable of interrupts
	TimerIntDisable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
	
	TimerDisable(TIMER1_BASE, TIMER_A);
	TimerClockSourceSet(TIMER1_BASE, TIMER_CLOCK_SYSTEM);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_A_PERIODIC);

	TimerLoadSet(TIMER1_BASE, TIMER_A, TIMER_reload_calculator(50));	//Set cycle nr for 300 ms	
	//TimerEnable(TIMER1_BASE, TIMER_A);  //Timer will be enabled by GPIO switch ISR
	
	TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
	IntPrioritySet(INT_TIMER1A,(TIMER1_PRIO)<<5);  //Priority 1 = "001"0.0000
	IntEnable(INT_TIMER1A);	//Wide Timer 0A enable of interrupts
}

void TIMER_2_Init(void)	//cyclic timer configuration for PF4 / SW1 debounce
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);  //Wide Timer 0 enable 
	IntDisable(INT_TIMER2A);  //Wide Timer 0A disable of interrupts
	TimerIntDisable(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
	
	TimerDisable(TIMER2_BASE, TIMER_A);
	TimerClockSourceSet(TIMER2_BASE, TIMER_CLOCK_SYSTEM);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_A_PERIODIC);

	TimerLoadSet(TIMER2_BASE, TIMER_A, TIMER_reload_calculator(50));	//Set cycle nr for 300 ms	
	//TimerEnable(TIMER2_BASE, TIMER_A);  //Timer will be enabled by GPIO switch ISR
	
	TimerIntEnable(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
	IntPrioritySet(INT_TIMER2A,(TIMER2_PRIO)<<5);  //Priority 1 = "001"0.0000
	IntEnable(INT_TIMER2A);	//Wide Timer 0A enable of interrupts
}

void TIMER1A_Handler(void)  //Timer 1 A ISR used to debounce SW2
{
	if(TimerIntStatus(TIMER1_BASE,false))
	{
		TimerDisable(TIMER1_BASE, TIMER_A);
		TimerIntClear(TIMER1_BASE, TIMER_A);
		//..	
	}
}

void TIMER2A_Handler(void)  //Timer 2 A ISR used to debounce SW1
{
	if(TimerIntStatus(TIMER2_BASE,false))
	{
		TimerDisable(TIMER2_BASE, TIMER_A);
		TimerIntClear(TIMER2_BASE, TIMER_A);
		//..
	}
}

//EOF
