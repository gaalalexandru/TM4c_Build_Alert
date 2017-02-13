#ifndef PWM_handler
#define PWM_handler
/*-------------------Type Includes-------------------*/
#include "stdbool.h"
#include "stdint.h"


/*Step3 init PWM for RGB LED and Duty Cycle changing function*/
void RGB_PWM_Init(unsigned long PWM_Period);
void RED_PWM_DutyCycle(unsigned long duty_cycle);
void GREEN_PWM_DutyCycle(unsigned long duty_cycle);
void BLUE_PWM_DutyCycle(unsigned long duty_cycle);
#endif
//EOF
