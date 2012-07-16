/*
 * UART_Functions.c
 * Provides functions that deal with UART I/O.
 * Author: Joshua James
 *
 */
 
#include <stdlib.h>				// malloc & free
#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "utils/ustdlib.h"
#include "RASLib/timer.h"

//for watchdog
#include "inc/hw_ints.h"		// input/output over UART
#include "driverlib/watchdog.h"
#include "driverlib/interrupt.h"
#include "RASLib/init.h"

#include "RASLib/servo.h"

#include "UART_Functions.h"
#include "time_functions.h"
#include "motor_functions.h"
#include "settings.h"
#include "common.h"
#include "utils.h"
#include "adc_functions.h"
#include "RASLib/encoder.h"


void initUART(void) 
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	
	UARTStdioInit(0);
}

void getMessage(char* buffer, char size)
{
	UARTgets(buffer, size);
}

void sendMessage(char* message)
{
	UARTprintf("%s", message);
}

char getc(void) 
{
	char ch = UARTgetc();
	return ch;
}

void putc(char ch) 
{
	UARTprintf("%c",ch);
}

tBoolean charIsAvailable(void) 
{
	if (UARTCharsAvail(UART0_BASE))	
		return true;
	else
		return false;
}

void simpleCommTest(void)
{
	char buffer[80];
	signed char speed;
	signed char angle;
	
	//initWatchdog();
	
	//while(1)
	//{
		getMessage(buffer, 80);
		
		switch(buffer[0])
		{
			case 'S': speed = SATURATE(atoi(&buffer[1]), -128, 127);
					  resetWatchdogTimer();
					  break;
			case 'A': angle = SATURATE(atoi(&buffer[1]), -128, 127);
					  resetWatchdogTimer();
					  break;
			default : break;
		}
		
		//getting rid of warnings
		speed = speed;
		angle = angle; 
		
		//setMotorSpeeds(speed,speed,angle);
	//}
}

void handleCommMessage(void)
{
	char buffer[80];
	unsigned char function_code = 0;
	long i;
	
	getMessage(buffer, 80);
	
	//UARTprintf("MESSAGE GET!!%s\n", buffer);
	
	//if(checksumIsCorrect(buffer, 80))
	//{
		
		
		for(i = 0; i < CODE_LENGTH; i++) //generate hash of function code
		{
			function_code ^= buffer[CODE_START + i];
		}
		
		switch(function_code)
		{
			case SVLM: SetJaguarVoltage(LEFT_JAGUAR, SATURATE(atoi(&buffer[DATA_START]), -128, 127));
					   robotIsMoving = 1;
					   resetWatchdogTimer(); //we got a valid message, so they are still talking to us
					   break;
			case SVRM: SetJaguarVoltage(RIGHT_JAGUAR, SATURATE(atoi(&buffer[DATA_START]), -128, 127));
					   robotIsMoving = 1;
					   resetWatchdogTimer(); //we got a valid message, so they are still talking to us
					   break;
			case SVSM: SetServoPosition(STEERING_SERVO, 127 + SATURATE(atoi(&buffer[DATA_START]), -128, 127));
					   break;
			case SALS: SetServoPosition(LIDAR_SERVO, 127 + SATURATE(atoi(&buffer[DATA_START]), -128, 127));
					   break;
			default :  UARTprintf("UNRECOGNIZED MESSAGE: %s\n", buffer);
					   break;
		}
	//}
	//else
	//{
	//	UARTprintf("CHECKSUM BROKEN!!\n");
	//}
}

void sendData(void)
{
	//{ENCL:[Left encoder counts]_ENCR:[Right encoder counts]_STEA:[steering angle]_HOKA:[hokuyo angle]_TIME:[Timestamp]}:[Checksum]\n
	char string[100];
	
	lockCanon();
	
	usnprintf((char*)string, 100, "$ ENCL %d ENCR %d STEA %d HOKA %d TIME %d*", canon_left_position, 
																				canon_right_position,
																				canon_steering_position,
																				canon_hokuyo_position,
																				timestamp);
	unlockCanon();
	
	UARTprintf("%s%02x\n", (char*) string, makeChecksum((char*) string, 100));
}
