#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"
#include "RASLib/init.h"
#include "RASLib/motor.h"
#include "RASLib/timer.h"
#include "RASLib/servo.h"
#include "RASLib/linesensor.h"

#include "fan.h"

unsigned char isTripped = false;

void InitializeGPIOIntTest() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE , GPIO_PIN_2);
	
	GPIOIntTypeSet(GPIO_PORTA_BASE , GPIO_PIN_2, GPIO_RISING_EDGE);
	GPIOPinIntEnable(GPIO_PORTA_BASE , GPIO_PIN_2);
	IntEnable(INT_GPIOA);
    IntMasterEnable();
}

void GPIOAIntHandler(){
	isTripped = true;
	GPIOPinIntClear(GPIO_PORTA_BASE , GPIO_PIN_2);
}

void resetFanTrip(void) {
 	isTripped = false;
}

char isFanTripped(void) {
//code to take 1 sec of data
//analog read into array
//do root mean square
//int arr[1024];
    UARTprintf("ping");
	return isTripped;
}
