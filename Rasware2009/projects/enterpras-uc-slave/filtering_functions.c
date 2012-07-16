/*
 * filtering_functions.c
 * Provides functions for the filtering of data
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
#include "definitions.h"
#include "utils.h"
#include "RASLib/encoder.h"

void initFilters(void)
{
	
}

signed long mean_function(signed long array[], unsigned long num_values)
{
	long i;
	signed long sum = 0;
	
	for(i=0; i < num_values; i++)
	{
		sum += array[i];
		//UARTprintf("Values:%d\tsum:%d\tnum:%d\n", array[i], sum, i);
	}
	
	//UARTprintf("\n\n\n\n\n");
	
	return (sum / num_values);
}

signed long median_function(signed long array[], unsigned long num_values)
{
	unsigned long swap;
	long i;
	long j;
	
	for(i = 0; i < num_values; i++)
	{
		for(j = i + 1; j < num_values; j++)
		{
			if(array[i] > array[j])
			{
				swap = array[j];
				array[j] = array[i];
				array[i] = swap;
			}
		}
	}
	
	if(num_values % 2 == 0)
	{
		return (array[num_values / 2] + array[num_values / 2 - 1]) / 2;
	}
	else
	{
		return array[num_values / 2];
	}
}

void calculateCanon(void)
{				
	if(!canonLocked())
	{
		lockSensors();
		
		canon_left_position = GetEncoderCount(LEFT_ENCODER);//mean_function((signed long*) &left_encoder_data.buffer, left_encoder_data.num_values);
		canon_right_position = GetEncoderCount(RIGHT_ENCODER);//mean_function((signed long*) &right_encoder_data.buffer, right_encoder_data.num_values);
		canon_steering_position = mean_function((signed long*) &steering_pot_data.buffer, steering_pot_data.num_values);
		canon_hokuyo_position = mean_function((signed long*) &hokuyo_pot_data.buffer, hokuyo_pot_data.num_values);
		
		//TODO: Calculate velocities
		unlockSensors();
	}	
}

void sampleSensors(void)
{
	if(!sensorsLocked())
	{
		//push(GetEncoderCount(LEFT_ENCODER), (SensorBuffer*) &left_encoder_data);	//sample left encoder and push on array
		//push(GetEncoderCount(RIGHT_ENCODER), (SensorBuffer*) &right_encoder_data);	//sample right encoder and push on array
		push(getCurrentSteeringAngle(), (SensorBuffer*) &steering_pot_data);		//sample steering pot and push on array
		push(getCurrentHokuyoAngle(), (SensorBuffer*) &hokuyo_pot_data);			//sample hokuyo pot and push on array
	}
}

signed long filterSteering(signed long newValue)
{
	push(newValue, (SensorBuffer*) &steering_pot_data);
	calculateCanon();
	return canon_steering_position;
}



/*unsigned long averageFilter(unsigned long newValue) 
{
	//long i;
	unsigned long average = 0;
	long numEntries;
	
	//update buffer
	averagingFilterBuffer[averagingFilterIndex] = newValue;
	if(averagingFilterIndex == AVERAGING_FILTER_SIZE - 1)
	{
		averagingFilterIndex = 0;
		averagingFilterBufferFull = 1;
	}
	else
	{
		averagingFilterIndex++;
	}
	
	//calculate average
	if(averagingFilterBufferFull)
	{
		numEntries = AVERAGING_FILTER_SIZE;
	}
	else
	{
		numEntries = averagingFilterIndex;
	}
	
	average = mean_function((unsigned long*) averagingFilterBuffer, numEntries);
	//UARTprintf("average:\t%d\n\n", average);
		
	return average;
}*/
