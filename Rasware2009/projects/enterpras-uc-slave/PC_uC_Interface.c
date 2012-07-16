/*
 * PC_uC_Interface.c
 * Provides the microcontroller side of the link between the PC and the microcontroller
 * Author: Joshua James
 *
 */
 
//Includes 
#include "inc/hw_types.h"

#include "RASLib/init.h"
#include "RASLib/motor.h"
#include "RASLib/servo.h"
#include "RASLib/encoder.h"
#include "RASLib/timer.h"

#include "UART_Functions.h"
#include "message_processor.h"
#include "nunchuck_functions.h"
#include "motor_functions.h"
#include "time_functions.h"
#include "settings.h"
#include "definitions.h"
#include "adc_functions.h"
#include "gpio_functions.h"
#include "sorted_buf.h"
#include "utils.h"
#include "common.h"
#include "filtering_functions.h"
#include "driverlib/gpio.h" 
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART

volatile unsigned char paradigm;
volatile unsigned char control_mode;
volatile unsigned char drive_type; 
volatile unsigned char battery_voltage;
volatile unsigned char steering_pider;
volatile unsigned char drive_pider;

//Functions
void init()
{
	//prevent lockout
	LockoutProtection();
	
	//initialize all physical sub-systems
	InitializeMCU();
	initUART();
	
	battery_voltage = 24;
	drive_type = ACKERMANN;
	steering_pider = COMPUTER;
	drive_pider = COMPUTER;
	paradigm = SERVO_SLAVE;
	
	if(paradigm == SERVO_SLAVE)
	{
		InitializeServos();
		InitializeMotors(false, false);
		initWatchdog();
		InitializeI2C();
		initNunchuck();
		initGPIO();
		control_mode = controlSwitchPosition();
		initTimers();
	}
	else if(paradigm == ADC_SLAVE)
	{
		UARTprintf("\n-------------------------------------------------------\n");
		UARTprintf("Initializing with paradigm %s...", (paradigm==ADC_SLAVE)? "ADC_SLAVE":"SERVO_SLAVE");
		initTimers();
		InitializeEncoders(true, false);
		initADC();
		initFilters();
	}
}

int main()
{
	init(); //do all necessary initializations (including interrupts)
	if(paradigm == ADC_SLAVE)
	{
		UARTprintf("Initialization complete!\n");
	}
	
	while(1)
	{
		if(paradigm == SERVO_SLAVE)
		{
			control_mode = controlSwitchPosition();
			if(control_mode == AUTONOMOUS)
			{
				if(charIsAvailable()) //if someone is trying to talk to us, go figure out what they want.
				{
					handleCommMessage();
				}
			}
			else if(control_mode == NUNCHUCK)
			{
				joyDrive(getNunchuckData()); //otherwise, be joyous!
			}
			else //something really bad happened, so lets try nunchucking it again
			{
				control_mode = NUNCHUCK;
			}
		}
	}
}
