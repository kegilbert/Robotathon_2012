#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

#include "RASLib/adc.h"


void InitAdcPorts(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
	ADCSequenceConfigure(ADC_BASE,0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC_BASE, 0, 1, ADC_CTL_CH1);
	ADCSequenceStepConfigure(ADC_BASE, 0, 2, ADC_CTL_CH2);
	ADCSequenceStepConfigure(ADC_BASE, 0, 3, ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END);
	ADCSequenceEnable(ADC_BASE, 0);
}

long sampleAdcPort(int port) {
	unsigned long ADCValues[4] = {0};
	ADCProcessorTrigger(ADC_BASE, 0 ); 
	while(!ADCIntStatus(ADC_BASE, 0, false)); 
	ADCSequenceDataGet(ADC_BASE, 0, ADCValues);
	ADCIntClear(ADC_BASE, 0);
	return ADCValues[port];
}
