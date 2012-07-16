//relay.c
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "relays.h"
#include "RASLib/timer.h"
#include "utils/uartstdio.h"

void InitializeRelays(void){ 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7, 0);
}

void CapacitorsSeries(void){
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_6);
}

void CapacitorsParalell(void){
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0x00);
}

void WindOn(void){
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7 | GPIO_PIN_4, GPIO_PIN_7);
}

void SolarOn(void){
	
	//GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7 | GPIO_PIN_4, GPIO_PIN_7 | GPIO_PIN_4);
	while(1){
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7, 0);
		UARTprintf("0");
		Wait(2000);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4 , GPIO_PIN_4);
		UARTprintf("1");
		Wait(2000);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_5 , GPIO_PIN_5);
		UARTprintf("2");
		Wait(2000);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 , GPIO_PIN_6);
		UARTprintf("3");
		Wait(2000);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7 , GPIO_PIN_7);
		UARTprintf("4");
		Wait(2000) ;
	}
}

void ElectricOn(void){
	 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_7, GPIO_PIN_4 );
}

void Discharge(void ){
	 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7, 0x00);
}

