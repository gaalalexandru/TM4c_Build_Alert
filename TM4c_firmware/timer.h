#ifndef TIMER
#define TIMER

/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-----------External function definisions-----------*/

/*Step1 - Init Wide Timer0 and write Wide Timer0 ISR*/
unsigned long TIMER_reload_calculator(unsigned long delay_time_ms);
void TIMER_Wide_0_Init(void);

/*Step3 - Init Timer 1 and 2 to GPIO switch debouncing, write Timer 1 and 2 ISR*/
void TIMER_1_Init(void);
void TIMER_2_Init(void);

#endif
//EOF
