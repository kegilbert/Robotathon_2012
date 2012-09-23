#define DEBUG
#define USE_RASLIB 1
#define MAX_VAL 350
#define INV_L true
#define INV_R false

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
#ifdef USE_RASLIB
#include "RASLib/servo.h"
#endif

#define InitializeUART()										\
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				\
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	\
	UARTStdioInit(0);


//these variables are volatile because they can be changed in interrupts

volatile tBoolean power;
volatile signed long forward;
volatile signed long sideward;

signed char flip0, flip1;

#if USE_RASLIB
void init_motors(tBoolean inv0, tBoolean inv1) {
	flip0 = inv0 ? -1 : 1;
	flip1 = inv1 ? -1 : 1;
	InitializeServos();
}

void set_motors(signed char m0, signed char m1) {
	m0 *= flip0;
	m1 *= flip1;
	SetServoPosition(SERVO_0, m0+128);
	SetServoPosition(SERVO_1, m1+128);
}
#else
//Chris had to eat his words
//
//pwm isn't what he thought it was
//rendering the following code not very useful
void init_motors(tBoolean inv0, tBoolean inv1) {
    //GPIO D pin 0 and 1 is for PWM signal
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	
    //Create the PWM signal
	PWMGenConfigure(PWM_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 256);
	PWMOutputInvert(PWM_BASE, PWM_OUT_0_BIT, inv0);
	PWMOutputInvert(PWM_BASE, PWM_OUT_1_BIT, inv1);
	PWMOutputState(PWM_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, 128);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, 128);
	PWMGenEnable(PWM_BASE, PWM_GEN_0);
}

void set_motors(signed char m0, signed char m1) {
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, m0+128);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, m1+128);
}
#endif

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
	signed long ml, mr, sc_for, sc_side;
	
	LockoutProtection();
	InitializeMCU();
	InitializeUART();
	IntMasterEnable();
	
	init_motors(INV_L, INV_R);
	init_input();
	
	UARTprintf("- Hi I am Couch! -");
	
	for (;;) {
#ifdef DEBUG
        //range of values is -512 to 512
		UARTprintf("[ f:%d s:%d p:%d ]\n",forward,sideward,power);
#endif
		
		if (!power) {
            //scale the motors to match MAX_VA
			sc_for = (forward * 512)/MAX_VAL;
			sc_side = (sideward * 512)/MAX_VAL;
	        
            //reduce the motor you're turning into
			if (sc_side > 0) {
				ml = (sc_for * (512-sc_side))/512;
				mr = sc_for;
			} else {
				ml = sc_for;
				mr = (sc_for * (512+sc_side))/512;
			}
             
            //shift to put the motors in the range of a char
			ml >>= 2;
			mr >>= 2;
            //check for overflow
			if (ml >  126) ml =  126;
			if (ml < -127) ml = -127;
			if (mr >  126) mr =  126;
			if (mr < -127) mr = -127;
			
			set_motors((signed char)ml, (signed char)mr);
		}
	}
}

