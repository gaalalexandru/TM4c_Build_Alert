#ifndef LED
#define LED

/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-------------------Macro Definitions----------------*/
#define DARK (0x00)
#define RED (0x02)	//PF1
#define BLUE (0x04)	//PF2
#define GREEN (0x08)  //PF3
#define YELLOW (0x0A)
#define SKY_BLUE (0x0C)
#define WHITE (0x0E)
#define PINK (0x06)

#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))

/*-----------External function definisions-----------*/
/*Step1 Init GPIO for RED LED and write toggle function*/
void GPIO_PortF_Init(void);
void GPIO_PortF_Toggle(unsigned char output);
#endif
//EOF
