// ************************************
// TIMER AND WAIT FUNCTIONS AND DEFINES
// ************************************

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"

#include "RASLib/init.h"
#include "RASLib/timer.h"

// Summary:	waits a specified amount of time
// Parameters:
//		ms: the number of microseconds to wait
// Note:	Uses TIMER1. If you use Wait() or WaitUS(), you should not use TIMER1 elsewhere in your code;
//			If you use TIMER1 in your code, you should not use Wait() or WaitUS()
void WaitUS(unsigned long us)
{							
	unsigned long ticks;
	static tBoolean isTimerInitialized = false;
	if(!isTimerInitialized)
	{
//		PRINT("init-ing timer"); NL;
		// first time initialization
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
		TimerConfigure(TIMER1_BASE, (TIMER_CFG_32_BIT_OS));
		isTimerInitialized = true;	
	}

//	PRINT("start-ing timer"); NL;
	// start the timer
	ticks = us*g_ulTicksPerUs;
	TimerLoadSet(TIMER1_BASE, TIMER_A, ticks);		// set initial value to count down from		
	TimerEnable(TIMER1_BASE, TIMER_A);		 

//	PRINT("wait-ing"); NL;
	// wait for the timer to count down
	// note: timer resets to and stops at the start value after it reaches 0, so check for that
	while(TimerValueGet(TIMER1_BASE, TIMER_A) != ticks);		  // spin
	TimerDisable(TIMER1_BASE, TIMER_BOTH);
//	PRINT("whoohoo!!!"); NL;
}

// Summary:	waits a specified amount of time
// Parameters:
//		ms: the number of milliseconds to wait
// Note:	Uses TIMER1. If you use Wait() or WaitUS(), you should not use TIMER1 elsewhere in your code;
//			If you use TIMER1 in your code, you should not use Wait() or WaitUS()
void Wait(unsigned long ms)
{
//PRINT_U(ms); TAB; PRINT_U(ms*1000L); NL;
	WaitUS(ms*1000L);
}

