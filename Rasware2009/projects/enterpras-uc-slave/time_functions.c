/*
 * time_functions.c
 * Provides ISRs for watchdog timeouts, timestamps, and other timer interrupts.
 * Author: Joshua James, Nicu Stiurca
 *
 */
 
#include <stdlib.h>				// malloc & free
#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

//for watchdog
#include "inc/hw_ints.h"		
#include "driverlib/watchdog.h"
#include "driverlib/interrupt.h"

//for timers
#include "driverlib/timer.h"

//for encoders
#include "RASLib/encoder.h"

#include "RASLib/init.h"
#include "RASLib/servo.h"

#include "motor_functions.h"
#include "time_functions.h"
#include "definitions.h"
#include "settings.h"
#include "utils.h"
#include "filtering_functions.h"
#include "adc_functions.h"
#include "uart_functions.h"
#include "common.h"
#include "gpio_functions.h"

#include "utils/ustdlib.h"

void resetWatchdogTimer(void)
{
	WatchdogReloadSet(WATCHDOG0_BASE, WATCHDOG_PERIOD);
}

void WatchdogIntHandler(void)
{
	WatchdogIntClear(WATCHDOG0_BASE);
	
	//stop all motors on the robot
	SetJaguarVoltage(LEFT_JAGUAR, 0);
	SetJaguarVoltage(RIGHT_JAGUAR, 0);
	
	
	//SetJaguarVoltage(STEERING_SERVO, 0);  //commented out because this causes motion instead of stopping it
	//SetServoPosition(LIDAR_SERVO, 0);	 
	
	robotIsMoving = 0;
}

void initWatchdog(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
	IntEnable(INT_WATCHDOG);
	resetWatchdogTimer();
	WatchdogEnable(WATCHDOG0_BASE);
}

void initTimers(void)
{
	//Timer 2A is for timestamps
	//Timer 2B is for encoder sampling and calculations
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    TimerConfigure(TIMER2_BASE, TIMER_CFG_16_BIT_PAIR| TIMER_CFG_A_PERIODIC);
	
	if(paradigm == ADC_SLAVE)
	{
    	TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet()/TICK_RATE); 	//1Khz = 1ms period
	}
	else
	{
		TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet()/TICK_RATE); 	
	}
	//TimerLoadSet(TIMER2_BASE, TIMER_B, SysCtlClockGet()/DATA_RATE);		//10Hz = 100ms period
	
    TimerEnable(TIMER2_BASE, TIMER_A);
	//TimerEnable(TIMER2_BASE, TIMER_B);
	
	IntPrioritySet(INT_TIMER2A, 1);
	//IntPrioritySet(INT_TIMER2B, 3);
	
	TimerIntEnable(TIMER2_BASE, TIMER_A);
	//TimerIntEnable(TIMER2_BASE, TIMER_B);
	
	IntEnable(INT_TIMER2A);	
	//IntEnable(INT_TIMER2B);
	
	IntMasterEnable();
}

/*void initFlasherTimer(void)
{	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_B, SysCtlClockGet()/TICK_RATE); 	//1Khz = 1ms period
    TimerEnable(TIMER1_BASE, TIMER_B);
	IntPrioritySet(INT_TIMER1B, 3);
	TimerIntEnable(TIMER1_BASE, TIMER_B);
	IntEnable(INT_TIMER1B);	
	IntMasterEnable();
}*/

void timestampInterruptHandler(void)
{
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	timestamp++;
	
	if(paradigm == ADC_SLAVE)
	{	
		if(timestamp % (TICK_RATE / SAMPLE_RATE) == 0)
		{
			sampleSensors();
		}
		
		if(timestamp % (TICK_RATE / DATA_RATE) == 0)
		{
			calculateCanon();
			sendData();
		}
	}
	else
	{
		if(timestamp % (TICK_RATE / FLASH_RATE) == 0)
		{
			if(robotIsMoving)
			{
				toggleWarningLight();
			}
			else
			{
				turnOnWarningLight();
			}
		}
	}
}

void flashInterruptHandler(void)
{
	if(robotIsMoving)
	{
		toggleWarningLight();
	}
	else
	{
		turnOnWarningLight();
	}
}
