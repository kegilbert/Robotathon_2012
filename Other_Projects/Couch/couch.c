#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "utils/uartstdio.h"	// input/output over UART

#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"

#include "RASLib/init.h"


#define InitializeUART()										\
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				\
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	\
	UARTStdioInit(0);

#define DEBUG	

//these variables are volatile because they can be changed in interrupts

volatile tBoolean power;
volatile signed long forward;
volatile signed long sideward;

void init_motors(tBoolean inv0, tBoolean inv1) {
    //GPIO C pin 7 and
    //GPIO B pin 5 are disables for the H-bridge
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //GPIO E pin 0 and 1 is for the PWM signal
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	
    //Create the PWM signal
	PWMGenConfigure(PWM_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM_BASE, PWM_GEN_2, 256);
	PWMOutputInvert(PWM_BASE, PWM_OUT_4_BIT, inv0);
	PWMOutputInvert(PWM_BASE, PWM_OUT_5_BIT, inv1);
	PWMOutputState(PWM_BASE, PWM_OUT_4_BIT | PWM_OUT_5_BIT, true);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_4, 0);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_5, 0);
	PWMGenEnable(PWM_BASE, PWM_GEN_2);
	
    //Set C7 and B5 low to enable the H-Bridge
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
}

void power_motors(tBoolean enabl) {
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, (!enabl) << 7);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, (!enabl) << 5);
}

void set_motors(signed char m0, signed char m1) {
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_4, m0+128);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_5, m1+128);
}

void init_input() {
    //GPIO D pin 7 is for the power signal
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
    //Create an interrupt that triggers and calls gpiod_handler 
    //on both edges of D7 signal
	GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_BOTH_EDGES);
	GPIOPinIntEnable(GPIO_PORTD_BASE, GPIO_PIN_7);
	IntEnable(INT_GPIOD);

    //ADC channel 0 and 1 are for the controller inputs
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
    //Create a sequence that reads both channel 0 and then channel 1
    //then triggers and interrupt that calls adc_handler
	ADCSequenceConfigure(ADC_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC_BASE, 0, 0, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC_BASE, 0, 1, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH1);
	ADCIntEnable(ADC_BASE, 0);
	IntEnable(INT_ADC0SS0);
	ADCSequenceEnable(ADC_BASE, 0);
    //Then begin the sequence
	ADCProcessorTrigger(ADC_BASE, 0);

    //interrupt handlers are defined in Startup.s
}

void gpiod_handler() {
	GPIOPinIntClear(GPIO_PORTD_BASE, GPIO_PIN_7);
	power = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7) >> 7;
}

void adc_handler() {
	unsigned long temp[8];
	ADCIntClear(ADC_BASE, 0);
	
    //The sequence data can be up to 8 values and is buffered
    //but because we pull the values everytime they are updated,
    //there should only ever be 2
	ADCSequenceDataGet(ADC_BASE, 0, temp);
	forward  = (signed long)(temp[0]-512);
	sideward = (signed long)(temp[1]-512);
	
    //Then trigger the ADC sequence again
	ADCProcessorTrigger(ADC_BASE, 0);
}


int main() {
	LockoutProtection();
	InitializeMCU();
	InitializeUART();
	IntMasterEnable();
	
	init_motors(true, true);
	init_input();
	
	UARTprintf("- Hi I am Couch! -");
	
	for (;;) {
        signed long ml,mr;
#ifdef DEBUG
        //range of values is -512 to 512
		UARTprintf("[ f:%d s:%d p:%d ]\n",forward,sideward,power);
#endif
		
        power_motors(power);

        if (sideward > 0) {
            ml = (forward * (512-sideward))/512;
            mr = forward;
        } else {
            ml = forward;
            mr = (forward * (512+sideward))/512;
        }

		set_motors((signed char)(ml >> 2), (signed char)(mr >> 2));
	}
}

