// *********************************
// LINE SENSOR FUNCTIONS AND DEFINES
//
// Usage notes: these functions
// assume the following pin mappings
// PB2-PB3	: Sensors 1-2
// PD2-PD7	: Sensors 3-8
// *********************************
#ifndef LINESENSOR_H
#define LINESENSOR_H

#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "timer.h"

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif
		  
#define NUM_SENSORS (8)					// the number of sensors
typedef unsigned char linesensor_t;		// the line sensor data type

// Summary: Initializes the appropriate GPIO
// Note:	Always call this function before any other sensor-related functions
//			Initializes dischargeTime to a default value of 1000 us

extern void InitializeLineSensor(void);
// Summary:	Same as above, but sets up the line sensor for asynchronous operation.
// 			This means the line sensor will update automatically in the background
// Note:	Avoid mixing both normal and asynchronous operation
// Note:	Uses TIMER0. If you use asynchronous line sensing, you should not use TIMER0 elsewhere in your code;
//			If you use TIMER0 in your code, you should not use asynchronous line sensing.
extern void InitializeLineSensorAsync(void);

// Summary: Sets the discharge time
// Parameters:
//		dischargeTime: the new discharge time 
//				constraints: 120 <= dischargeTime <= 2600
// Note:	This function applies to both normal and asynchronous operation
extern void SetDischargeTime(unsigned long dischargeTime);

// Summary:	Reads the current line sensor values
// Note:	this method takes 10 us + dischargeTime to execute
// Returns:	a package of bits corresponding to the current readings from the line sensor
extern linesensor_t ReadLineSensor(void);

// Summary:	Same as above, but immediately returns the last automatically-updated reading
// Note:	Uses TIMER0. If you use asynchronous line sensing, you should not use TIMER0 elsewhere in your code;
//			If you use TIMER0 in your code, you should not use asynchronous line sensing.
extern linesensor_t ReadLineSensorAsync(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // LINESENSOR_H
