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
	
	while(1) {	
	  //Put looping code
	}

}
