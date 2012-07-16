/*
 * adc_functions.c
 * Provides functions for control of devices attached to the adc ports.
 * Author: Joshua James
 *
 */
 

#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"		// input/output over UART
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "settings.h"
#include "definitions.h"
#include "sorted_buf.h"
#include "utils.h"
#include "filtering_functions.h"
#include "RASLib/timer.h"

#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
 
/*static int n = 0;
static sorted_buf_t sortedInput;
static tBoolean sortedInit = 0; */
 
void initADC(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
	
	ADCSequenceConfigure(ADC0_BASE,POTENTIOMETER, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, POTENTIOMETER, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
	ADCSequenceEnable(ADC0_BASE, POTENTIOMETER);
	
	ADCSequenceConfigure(ADC0_BASE, HOKUYO, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, HOKUYO, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2);
	ADCSequenceEnable(ADC0_BASE, HOKUYO);
	
	/*ADCSequenceConfigure(ADC0_BASE,2, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2);
	ADCSequenceEnable(ADC0_BASE, 2);
	
	ADCSequenceConfigure(ADC0_BASE,3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH3);
	ADCSequenceEnable(ADC0_BASE, 3);*/
	
	/*
	ADCSequenceConfigure(ADC0_BASE,3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH1);
	ADCSequenceEnable(ADC0_BASE, 3);
	*/
}

unsigned long getADC0(void)
{
	unsigned long potValue;
	
	ADCProcessorTrigger(ADC0_BASE, 0); 				//ask for pot.
	while(!ADCIntStatus(ADC0_BASE, 0, false)); 		//wait 'till we get it.
	ADCSequenceDataGet(ADC0_BASE, 0, &potValue);	//once we have it,
	
	return potValue;											//and distribute it!
}

unsigned long getADC1(void)
{
	unsigned long potValue;
	
	ADCProcessorTrigger(ADC0_BASE, 1); 				//ask for pot.
	while(!ADCIntStatus(ADC0_BASE, 1, false)); 		//wait 'till we get it.
	ADCSequenceDataGet(ADC0_BASE, 1, &potValue);	//once we have it,
	
	return potValue;											//and distribute it!
}

unsigned long getADC2(void)
{
	unsigned long potValue;
	
	ADCProcessorTrigger(ADC0_BASE, 2); 				//ask for pot.
	while(!ADCIntStatus(ADC0_BASE, 2, false)); 		//wait 'till we get it.
	ADCSequenceDataGet(ADC0_BASE, 2, &potValue);	//once we have it,
	
	return potValue;											//and distribute it!
}

unsigned long getADC3(void)
{
	unsigned long potValue;
	
	ADCProcessorTrigger(ADC0_BASE, 3); 				//ask for pot.
	while(!ADCIntStatus(ADC0_BASE, 3, false)); 		//wait 'till we get it.
	ADCSequenceDataGet(ADC0_BASE, 3, &potValue);	//once we have it,
	
	return potValue;											//and distribute it!
}

unsigned long getCurrentSteeringAngle(void)
{
	return getADC0();
}

unsigned long getCurrentHokuyoAngle(void)
{
	return getADC1();
}
