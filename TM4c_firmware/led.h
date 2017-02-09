#ifndef LED
#define LED

/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-------------------Macro Definitions----------------*/
#define RED (0x02)	//PF1
#define BLUE (0x04)	//PF2
#define GREEN (0x08)  //PF3

/*-----------External function definisions-----------*/
/*Step1 Init GPIO for RED LED and write toggle function*/
void RGB_Led_Init(void);
void RGB_Led_toggle(unsigned char color);

/*Step3 init PWM for RGB LED and Duty Cycle changing function*/
void RGB_PWM_Init(unsigned long PWM_Period);
void RED_PWM_DutyCycle(unsigned long duty_cycle);
void GREEN_PWM_DutyCycle(unsigned long duty_cycle);
void BLUE_PWM_DutyCycle(unsigned long duty_cycle);

#endif
//EOF
