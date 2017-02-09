/*------------------Project Includes-----------------*/
#include "led.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"

/*-------------------Macro Definitions----------------*/
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02 - 1
// blue     --B    0x04 - 2
// green    -G-    0x08 
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06
#define PWM_disable (false)			//Switch to disable PWM output
#define PWM_enable (true)				//Switch to enable PWM output
	
/*-------------------Function Definitions-------------*/
/*Step1 Init GPIO for RED LED and write toggle function*/

void RGB_Led_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  //Enable clock on port F
  GPIO_PORTF_CR_R |= 0x1F;  //Allow changes to PF4-0
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  //Set PF1,2,3 GPIO Output
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_DIR_MODE_OUT);  //Set direction Output for PF1,2,3
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);  //Configure PF1,2,3
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);  //Set initially to 0
}

void RGB_Led_toggle(unsigned char color)
{
	static unsigned char status = 1;
	if(status) {
		GPIOPinWrite(GPIO_PORTF_BASE, color, color);	//switch on desired color	
	}
	else {
		GPIOPinWrite(GPIO_PORTF_BASE, color, 0);  //switch off
	}
	status ^= 1;  //toggle status
}

/*Step3 init PWM for RGB LED and Duty Cycle changing function*/

void RGB_PWM_Init(unsigned long PWM_Period)
{
	//PF1 is Red
	//PF2 is Blue
	//PF3 is Green
	
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);  //Enable clock for PWM
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //PWM enable 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  //Port F enable
	
	GPIOPinConfigure(GPIO_PF1_M1PWM5);  //Configure PF1, Function LED toggle
	GPIOPinConfigure(GPIO_PF2_M1PWM6);  //Configure PF2, Function LED toggle
	GPIOPinConfigure(GPIO_PF3_M1PWM7);  //Configure PF3, Function LED toggle
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);  //Set PWM for PF1, Function LED toggle
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);  //Set PWM for PF2, Function LED toggle
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);  //Set PWM for PF3, Function LED toggle
	
	PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);	//Configure Generator 2
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, PWM_Period);  //Configure Generator 2 period (frequency)
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);	//Configure Generator 3
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, PWM_Period);  //Configure Generator 3 period (frequency)
	
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (50*(PWMGenPeriodGet(PWM1_BASE,PWM_GEN_2))/100));//Set initial duty cycle 10%
	PWMGenEnable(PWM1_BASE, PWM_GEN_2);  //Enable Generator 2
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, (50*(PWMGenPeriodGet(PWM1_BASE,PWM_GEN_3))/100));//Set initial duty cycle 10%
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);	 //Enable Generator 3
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, (50*(PWMGenPeriodGet(PWM1_BASE,PWM_GEN_3))/100));//Set initial duty cycle 10%
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);  //Enable Generator 3
	PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT , PWM_enable);
	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT , PWM_enable);
	PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT , PWM_enable); // Only if want to vary the GREEN led
}

void RED_PWM_DutyCycle(unsigned long duty_cycle)
{
		if(duty_cycle >= 99)	
		{
			duty_cycle = 99;	//Set duty cycle to 99% if request is greater than 99%
		}
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (duty_cycle*(PWMGenPeriodGet(PWM1_BASE,PWM_GEN_2))/100));	//Set duty cycle
		PWMGenEnable(PWM1_BASE, PWM_GEN_2); //Enable generator 2
		PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT , PWM_enable); //Enable PWM output on PF1 - Red LED
}

void GREEN_PWM_DutyCycle(unsigned long duty_cycle)  //Not used in this project
{
if(duty_cycle == 0)
	{
		PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT , PWM_disable);	//If requested duty cycle is 0% stop PWM output
		PWMGenDisable(PWM1_BASE, PWM_GEN_3);	//Disable generator 3 temporarly
	}
	else
	{
		if(duty_cycle >99)	
		{
			duty_cycle = 99;	//Set duty cycle to 99% if request is greater than 99%
		}
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, (duty_cycle*(PWMGenPeriodGet(PWM1_BASE,PWM_GEN_3))/100));	//Set duty cycle
		PWMGenEnable(PWM1_BASE, PWM_GEN_3); //Enable generator 3
		PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT , PWM_enable); //Enable PWM output on PF3 - Green LED
	}
}	


void BLUE_PWM_DutyCycle(unsigned long duty_cycle)
{
		if(duty_cycle >99)	
		{
			duty_cycle = 99;	//Set duty cycle to 99% if request is greater than 99%
		}
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, (duty_cycle*(PWMGenPeriodGet(PWM1_BASE,PWM_GEN_3))/100));	//Set duty cycle
		PWMGenEnable(PWM1_BASE, PWM_GEN_3); //Enable generator 3
		PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT , PWM_enable); //Enable PWM output on PF2 - Blue LED
}

//EOF
