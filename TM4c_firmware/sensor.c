#include "sensor.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

/*-------------------Macro Definitions----------------*/
#define ADC0SS3_PRIO (0x02)  //Priority 2
#define ADC_VREFP (3.3)  //2.5 Volt
#define ADC_VREFN (0)  //8 Volt
#define ADC_Ref_Voltage (ADC_VREFP - ADC_VREFN)	//33 represents 3v3, 50 represents 5v

unsigned long Sensor_Temperature = 0;
unsigned long Sensor_AnalogVoltage = 0;

/*Step2 - Init ADC to collect sensor information and write ADC0, sequencer3 ISR*/

void Sensor_Init(void) //Initialize temperature sensor input
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);  //The ADC0 peripheral must be enabled for use.

	IntDisable(INT_ADC0SS3);
	
	ADCIntClear(ADC0_BASE, 3);  //Clear interrupt flag	
	ADCIntDisable(ADC0_BASE, 3);
	ADCSequenceDisable(ADC0_BASE,3);  //Disable sequencer 3
	
	ADCSequenceConfigure(ADC0_BASE,3, ADC_TRIGGER_PROCESSOR, 0); //Sequence 3 will do a single sample when the processor (SW) sends a signal to start the conversion
	ADCSequenceStepConfigure(ADC0_BASE,3,0, ADC_CTL_CH3 |ADC_CTL_TS| ADC_CTL_IE | ADC_CTL_END);  //Configure step 0 on sequencer 3 to channel 1, interrupt enable and end conversion when after 1 conversion
	
	ADCSequenceEnable(ADC0_BASE,3);  //Enable sequencer 3
	ADCIntEnable(ADC0_BASE, 3); 
		
	IntPrioritySet(INT_ADC0SS3,(ADC0SS3_PRIO)<<5);
	IntEnable(INT_ADC0SS3);	
}

void ADC0SS3_Handler(void) {  //ADC0 Seq3 ISR

	uint32_t digital_value;
	if(ADCIntStatus(ADC0_BASE, 3, false))
	{
		ADCIntClear(ADC0_BASE, 3);  //Clear interrupt flag
		ADCSequenceDataGet(ADC0_BASE, 3, &digital_value);		
		
		IntMasterDisable();	//Global interrupt disable
		Sensor_Temperature = (147.5 - ((75 * (ADC_Ref_Voltage) * digital_value) / 4095));
		Sensor_AnalogVoltage = ((digital_value * ADC_Ref_Voltage)/4096)*1000;
		IntMasterEnable();	//Global interrupt enable
	}
}

//EOF
