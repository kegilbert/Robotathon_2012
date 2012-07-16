#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "charging.h"
#include "driverlib/sysctl.h"

#include "utils/uartstdio.h"
#include "travel.h"
#include "relays.h"

enum {NOT_CHARGING,CHARGING} charging;


void InitializeCharge(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
	ADCSequenceConfigure(ADC_BASE,2, ADC_TRIGGER_PROCESSOR, 1);
	ADCSequenceStepConfigure(ADC_BASE, 2, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2);
	ADCSequenceEnable(ADC_BASE, 2);
	charging = NOT_CHARGING;
}

long GetAnalog(int port)
{
	unsigned long ulADCValue;
	ADCProcessorTrigger(ADC_BASE, port ); 
	while(!ADCIntStatus(ADC_BASE, port, false)); 
	ADCSequenceDataGet(ADC_BASE, port, &ulADCValue);
	return ulADCValue;
}

#define CHARGING_THRESHOLD 20
void UpdateCapState(void){
	static int last3=0,last2=0,last1=0,last0=0;
	last3=last2;
	last2=last1;
	last1=last0;
	last0=GetAnalog(2);
	/*UARTprintf("New Data: %d\n", last0);
	UARTprintf("Charging? %d \n", Charging());	*/
	if( ( (last2-last3) + (last1-last2) + (last0-last1) ) > CHARGING_THRESHOLD ){
		 charging = CHARGING;
	}
	else{
		 charging = NOT_CHARGING;
	}
}

tBoolean Charging(void){
	return charging;
}

int GetCapacitorCharge(void){
	return GetAnalog(0);
}


void charge(unsigned char source) {
	switch(source) {
		case FLAG:
			//just sit and discharge
			//forever
			Discharge();
			CapacitorsSeries();
			for(;;);
			//break;
		case FAN:
			CapacitorsParalell();
			WindOn();
			while(Charging());
			Discharge();
			CapacitorsSeries();
			break;
		case ELECTRIC:	  
			CapacitorsParalell();
			ElectricOn();
			while(Charging());
			Discharge();
			CapacitorsSeries();
			break;
		case LIGHT:
			CapacitorsParalell();
			SolarOn();
			while(Charging());
			Discharge();
			CapacitorsSeries();
			break;
	}	
}




