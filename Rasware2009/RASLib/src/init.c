//*****************************************************************************
//
// init.h - Initialize headers for using the initialize functions
//
// Original author: Nicu Stiurca
// Date created: 8-5-2009
//
// Changelog:
// 08-05-2009: initial creation
//
//*****************************************************************************
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "RASLib/init.h"

//*****************************************************************************
//
// Global variables
//
//*****************************************************************************
unsigned long g_ulTicksPerSecond;
unsigned long g_ulTicksPerUs;
unsigned long g_ulPWMTicksPerSecond;

// Summary:	Provides protection against locking yourself out of being able
//		to reprogram the board
// Note:	Always call this first before doing ANYTHING
void LockoutProtection(void)
{
//	static int i;	// static prevents optimizer from removing this loop
//	for(i=0; i<1500000; ++i);

	// Check pin PC4
	// if it's low, it means the user wants to enter do-nothing loop
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);
	if(GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4))
		return;	// PC4 is normally wired to a pull-up resistor

	while(1);	// if it's not, loop forever
}

// Summary:	Initialize the MCU, including setting it to run from PLL,
//		enabling SysTick and interrupts.
void InitializeMCU(void)
{
/*	extern void SysTickPeriodSet(unsigned long);
	extern void SysTickEnable(void);//*/
	extern tBoolean IntMasterEnable(void);

	// Set the clocking to run from PLL, using external oscillator
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
		SYSCTL_XTAL_6MHZ);
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);	// PWM modules use 16-bit
		// registers, so use a divisor of 16 to prevent overflow

	g_ulTicksPerSecond = SysCtlClockGet();
	g_ulTicksPerUs = g_ulTicksPerSecond / 1000000;
	g_ulPWMTicksPerSecond = g_ulTicksPerSecond / 64;

/*	// Set up and enable the SysTick timer. It will be used as a reference
	// for delay loops in the interrupt handlers. The SysTick timer period
	// will be set up for one second.
	SysTickPeriodSet(g_ulTicksPerSecond);
	SysTickEnable();//*/

	IntMasterEnable();
}

