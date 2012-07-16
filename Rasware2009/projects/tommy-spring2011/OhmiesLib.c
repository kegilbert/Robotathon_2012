#include "inc/hw_types.h"               // tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"    // input/output over UART
#include "driverlib/uart.h"             // input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

#include "RASLib/motor.h"
//#include "xor-0.h"

void initIRSensor(void) {
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
        ADCSequenceConfigure(ADC_BASE,0, ADC_TRIGGER_PROCESSOR, 0);
        ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_CTL_CH0);
        ADCSequenceStepConfigure(ADC_BASE, 0, 1, ADC_CTL_CH1);
        ADCSequenceStepConfigure(ADC_BASE, 0, 2, ADC_CTL_CH2);
        ADCSequenceStepConfigure(ADC_BASE, 0, 3, ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END);
        ADCSequenceEnable(ADC_BASE, 0);
}

long getADCValue(int adcport) {
        unsigned long ADCValue[4] = {0};
        ADCProcessorTrigger(ADC_BASE, 0 ); 
        while(!ADCIntStatus(ADC_BASE, 0, false)); 
        ADCSequenceDataGet(ADC_BASE, 0, ADCValue);
        ADCIntClear(ADC_BASE, 0);
        return ADCValue[adcport];
}

//**Before use, Adjust value of left,right in Forward, TurnLeft, TurnRight based on motor
#define MaxSpeed	127	//speed rang from -128(full speed backward) to 127(full speed forward)
#define AdjustSpeed	80 //slow down 1 wheel to turn
signed char left = 0, right = 0;
void Forward(void){
	left = MaxSpeed;
	right = MaxSpeed;
	SetMotorPowers(right, left);	
}

void TurnLeft(void){
	left = AdjustSpeed;
	right = MaxSpeed;
	SetMotorPowers(right, left);	
}

void TurnRight(void){
	left = MaxSpeed;
	right = AdjustSpeed;
	SetMotorPowers(right, left);	
}
