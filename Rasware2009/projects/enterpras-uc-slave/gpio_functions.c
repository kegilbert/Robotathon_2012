/*
 * gpio_functions.c
 * Provides functions for control of devices attached to the gpio ports.
 * Author: Joshua James
 *
 */

#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"		// input/output over UART
#include "driverlib/gpio.h" 
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include "settings.h"
#include "definitions.h"
#include "gpio_functions.h"
#include "common.h"

static volatile unsigned char warning_light_output;
 
void initGPIO(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
 	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, CONTROL_TYPE_INPUT);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, WARNING_LIGHT);
	//turnOnWarningLight();
}

char controlSwitchPosition(void)
{
	return GPIOPinRead(GPIO_PORTA_BASE, CONTROL_TYPE_INPUT);
}

void turnOnWarningLight(void)
{
	warning_light_output = 0xff;
	GPIOPinWrite(GPIO_PORTD_BASE, WARNING_LIGHT, warning_light_output); 
}

void turnOffWarningLight(void)
{
	warning_light_output = 0x00;
	GPIOPinWrite(GPIO_PORTD_BASE, WARNING_LIGHT, warning_light_output); 
}

void toggleWarningLight(void)
{
	warning_light_output ^= 0xFF;
	GPIOPinWrite(GPIO_PORTD_BASE, WARNING_LIGHT, warning_light_output); 
}
