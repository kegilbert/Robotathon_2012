#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#include "cb_jumper.h"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

unsigned char Jumper_Value;

void Jumper_Init(void) {
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4);
	Jumper_Read();
}

unsigned char Jumper_Read(void) {
	return Jumper_Value = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4) >> 4;
}
