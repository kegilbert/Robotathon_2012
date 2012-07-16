#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "RASLib/motor.h"
#include "driverlib/adc.h"
#include "tommy-spring2011.h"

#define MaxSpeed	127	//speed rang from -128(full speed backward) to 127(full speed forward)
#define AdjustSpeed	80 //slow down 1 wheel to turn
#define SafeRange	600 //desired safety distance from side wall	large number = closer

//signed char left = 0, right = 0;

void MoveStraight(void) {
	long IR_Left;

	//Initialize Hardware
	InitializeMotors(true, true);
	initIRSensor();

	//Start by moving forward full speed
	Forward();

/*	while (1){
	TurnRight();
	UARTprintf("IR: %d\t\tSPEED Left: %d\t\tRight: %d\n",IR_Left,left,right);
	}*/
	while (1){
		IR_Left = getADCValue();
//		UARTprintf("IR: %d\t\tSPEED Left: %d\t\tRight: %d\n",IR_Left,left,right);
		if (IR_Left > SafeRange+50) TurnRight();
		else if (IR_Left < SafeRange-50) TurnLeft();
		else Forward();
	}  
}


