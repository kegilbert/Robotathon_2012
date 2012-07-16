#include "inc/hw_types.h"		// tBoolean
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "RASLib/init.h"
#include "RASLib/motor.h"
#include "utils/uartstdio.h"
#include "RASLib/timer.h"

#define let init();
#define the for
#define monkeys (;;)
#define roll run();
#define CHARGING_THRESHOLD 20


#include "travel.h"
#include "charging.h"
#include "panel.h"
#include "fan.h"
#include "ADS7830.h"
#include "clock.h"
#include "relays.h"

// Order of sources
// The order indicates which one we want first
#define BEST_SOURCE ELECTRIC

//
// Goes to location
//


//
//Initialization method
//
void init(void) {
	//Necessary inits for chip
	LockoutProtection();
	InitializeMCU();
	//Various driver inits
	//initUART
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	
	UARTStdioInit(0);

	//Initialize Franks interrupt
	//InitializeGPIOIntTest();
	
	initPanelServos();
	travelInit();
	InitializeCharge();
	InitializeClock();
	InitializeRelays();
	//raisePanel();
}

//
//Run method
//
void run(void) {
	UARTprintf("and?");
	testSensors();
}

	
//
//Don't put any important code in main
//Use main freely to test functions and stuff
//
//For competition main should simply call init and run
//
int main(void) {
	init();
		//TODO:connect something to the adc
	
	//gotoDest(ELECTRIC);
	//goForwardBlocking();
	//Wait(800);
	//while( ADS7830_Values[2] < 60); //Front IR sensor
	//SetMotorPowers(-85, -120);
	//wait(500);
	//UARTprintf("following");
	//WallFollow(2,10000,1);// TimeOUT 10 sec FORWARD
	//BackOut(); 
	
	//isFanTripped();
	//SolarOn();
	for (;;) run();
	//WallFollow(0,0,1); // WallFollow Forever, FORWARD
	//lowerPanel();
	 //testSensors();
	//UARTprintf("you shouldn't get here..");
}


