/*------------------Project Includes-----------------*/
#include "switch.h"
#include "led.h"
#include "timer.h"
#include "display.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

/*-------------------Macro Definitions----------------*/
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIOF_PRIO (0x01)  //GPIO Port F priority


unsigned long Simulated_Temperatur = 500;

/*Step3 - Init GPIO input for switches to simulate large temperature changes*/
/*Step3 - Write GPIO PortF ISR*/

void Switch_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  //Enable clock on port F
	
	GPIO_PORTF_LOCK_R = 0x4C4F434B;  //Unlock PortF PF0  
  GPIO_PORTF_CR_R |= 0x1F;  //Allow changes to PF4-0
	
	IntDisable(INT_GPIOF);  //GPIO Port F disable of interrupts
	
	GPIOIntDisable(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4);  //Disable GPIO pin interrupt
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);  //Set PF0 and PF4 as GPIO Input
	//GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_FALLING_EDGE);  //Set Falling edge interrupt type
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_LOW_LEVEL);  //Set Low level interrupt type
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_DIR_MODE_IN);  //Set direction input for PF0 and PF4
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //Configure PUR for PF0 and PF4
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_4);  //Enable GPIO pin interrupt
	
	IntPrioritySet(INT_GPIOF,(GPIOF_PRIO)<<5);  //Priority 2 = "010"0.0000
	IntEnable(INT_GPIOF);  //GPIO Port F enable of interrupts
}

void GPIOF_Handler(void) 	//GPIO port F ISR
{
	unsigned long switch_status = 0;
	//static int pwm_dc=0;

	switch_status = GPIOIntStatus(GPIO_PORTF_BASE,false);
	GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_0 | GPIO_INT_PIN_4);
	if((switch_status&(GPIO_INT_PIN_0 | GPIO_INT_PIN_4)) == (GPIO_INT_PIN_0 | GPIO_INT_PIN_4))
	{
		//Both switches are pressed
		//Do nothing
	}
	else if(switch_status & GPIO_INT_PIN_4)
	{
		//PF4
		TimerEnable(TIMER2_BASE, TIMER_A); //Enable Timer2
		GPIOIntDisable(GPIO_PORTF_BASE,GPIO_PIN_4);  //Disable GPIO pin interrupt	
	}
	else if(switch_status & GPIO_INT_PIN_0)
	{
		//PF0
		TimerEnable(TIMER1_BASE, TIMER_A); //Enable Timer1
		GPIOIntDisable(GPIO_PORTF_BASE,GPIO_PIN_0);  //Disable GPIO pin interrupt
	}
}

//EOF
