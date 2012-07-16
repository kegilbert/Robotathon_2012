/******************************************************************************************/
/*                                      MAIN                                              */
/******************************************************************************************/

// Kevin Gilbert
// Created: 16-07-2012, 11:00AM
// Last Updated: 16-07-2012, 11:15AM
//   Main program, created from Rasware2009 Template

#include "inc/hw_types.h"		// tBoolean
#include "TEMPLATE.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "RASLib/init.h"

int main(void)
{	
	LockoutProtection();
	InitializeMCU();
	initUART();																							    
<<<<<<< HEAD
	
	//Motor Init
        InitializeMotors(true,true);
=======
	/****************************************
	# IR sensors to navigate through field, #
	# or line sensors? Both?                #
	*****************************************/

        // while(rightSensor>=60&&rightSensor<=140) forward;
	// while..........................etc
	// # when IR reads the gap (all IR max range, line 
	// # sensor confirm, turn to push marbles
>>>>>>> 762d09796cfcc3710d5a74f1b14975a3a154b8e7

	while(1) {	
	  //Put looping code
		SetMotorPowers(80,80);    // Sample motor setup, test1
	}

}
