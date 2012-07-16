/*
 * utils.c
 * Provides general utilities for data manipulation.
 * Author: Joshua James
 *
 */
#include <string.h>
#include <stdlib.h>				// malloc & free
#include "inc/hw_types.h"		// tBoolean
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "settings.h"
#include "adc_functions.h"
#include "motor_functions.h"
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_ints.h"		
#include "driverlib/watchdog.h"
#include "driverlib/interrupt.h"
#include "utils/ustdlib.h"
#include "common.h"
#include "RASLib/init.h"
#include "utils.h"
#include "filtering_functions.h"
#include "RASLib/servo.h"
#include "RASLib/timer.h"

signed char turnDirection(signed char angle1, signed char angle2)
{
	return 0;
}

char angleBetween(signed char angle1, signed char angle2)
{
	return 0;
}

unsigned char makeChecksum(char* data, unsigned long buffer_size)
{
	long i = 1;
	unsigned char checksum = 0;
	
	while(data[i] != '*' && i < buffer_size)
	{
		checksum ^= data[i];
		i++;
	}
	
	return checksum;
}

unsigned char checksumIsCorrect(char* data, unsigned long buffer_size)
{
	long i = 1;
	unsigned char given_checksum;
	unsigned char real_checksum = makeChecksum(data, buffer_size);
	
	while(data[i] != '*')
	{
		if(i >= buffer_size)
		{
			return 0;
		}
		
		i++;
	}
	i++;	//i now points to the checksum
	given_checksum = data[i];
	
	return (real_checksum == given_checksum);
}

//char string[100]; //if you put this in the function
void pidGraph(void)
{
	//char string[50];
	//"$O[data]P[data]T[data]*[checksum]\n"
	//long position = getCurrentSteeringAngle();
	
	//try to move
	//pidSteeringServo(desired_steering_angle, position);
	
	//output stuff
	//usnprintf((char*)string, 50, "$O%dP%dT%d*", desired_steering_angle, position, timestamp);
	//UARTprintf("%s%2x\n", (char*) string, makeChecksum((char*) string, 50));
}

//push for sensor data buffers
void push(signed long value, SensorBuffer* sensorBuffer)
{
	sensorBuffer->buffer[sensorBuffer->index] = value;
	sensorBuffer->index = (sensorBuffer->index + 1);
	if(sensorBuffer->index >= FILTER_SIZE)
	{
		sensorBuffer->index = 0;
	}
	
	sensorBuffer->num_values = SATURATE(sensorBuffer->num_values + 1, 0, FILTER_SIZE);
	//UARTprintf("index:%d\n", sensorBuffer->index + 1);
	//UARTprintf("S:%d\n", mean_function((signed long*) &steering_pot_data.buffer, steering_pot_data.num_values));
}

void testServoPort(servo_t device)
{
	long i = 0;
	char direction = 0;
	
	while(1)
	{
		SetServoPosition(RIGHT_JAGUAR, i);
		SetServoPosition(LEFT_JAGUAR, i);
		SetServoPosition(STEERING_SERVO, i);
		SetServoPosition(LIDAR_SERVO, i);
		
		if(direction)
		{
			i++;
		}
		else
		{
			i--;
		}
		
		if(i >= 255 | i <= 0)
		{
			direction ^= 0;
		}
		
		Wait(100);
	}
}
