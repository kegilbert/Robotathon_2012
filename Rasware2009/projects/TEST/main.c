#include "inc/hw_types.h"               // tBoolean
#include "TEMPLATE.h"
#include "utils/uartstdio.h"    // input/output over UART
#include "driverlib/uart.h"             // input/output over UART
#include "RASLib/init.h"

/***************************************************************/
/*  Kevin Gilbert                                              */
//  Created: 20-09-2012, 11:00PM
//  Last Updated: 20-09-2012
//  
//  Test basic board functions -
//	+ Drive motors
//	+ Send voltages to GPIO
//
/****************************************************************/

void main(void) {
	
	LockoutProtection();
	InitializeMCU();
	initUART();

	UARTprintf(" --- Test_1 --- \n");
	UARTprintf(" ***** Rotate Left Motor Forward/Reverse ****** \n");
	
	signed char motorL, motorR;
	InitializeMotors(false,false);
	motorL=64; motorR=0;
	SetMotorPowers(motorL,motorR);
	Wait(3000);   
	motorL= -127;
	SetMotorPowers(motorL,motorR);
	Wait(3000);
	
	UARTprintf(" ***** Rotate Right Motor Forward/Reverse ***** \n");
	motorL=0; motorR=64;
	SetMotorPowers(motorL,motorR);
	Wait(3000);
	motorR=-127;
	SetMotorPowers(motorL,motorR);
	Wait(3000);

	/********************************************/
	// Initialize I/O, set to Output     
	// Set voltage to pins, test with voltmeter
	/********************************************/
}
