// clock.c

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
//#include "timer.h"
#include "charging.h"
#include "clock.h"
#include "ADS7830.h"


#define clockFreq 1000

unsigned long msEllapsed;

void InitializeClock(void){
	 	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
		TimerConfigure(TIMER2_BASE, TIMER_CFG_32_BIT_PER);
		TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet()/clockFreq);
		TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
		IntEnable(INT_TIMER2A);
		TimerEnable(TIMER2_BASE, TIMER_A);
		msEllapsed = 0;
}

unsigned long GetTimeMS(void){
	    return msEllapsed;
}

void Timer2InterruptHandler(void){
		TimerIntClear(TIMER2_BASE, TimerIntStatus(TIMER2_BASE,0));
		msEllapsed++;
		UpdateCapState();
		if(msEllapsed%50 == 0) ADS7830ReadNonBlocking();
}

