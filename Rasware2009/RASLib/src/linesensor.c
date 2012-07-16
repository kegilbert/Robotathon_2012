// *********************************
// LINE SENSOR FUNCTIONS AND DEFINES
//
// Usage notes: these functions
// assume the following pin mappings
// PB2-PB3	: Sensors 1-2
// PD2-PD7	: Sensors 3-8
// *********************************
#include "RASLib/linesensor.h"
#include "RASLib/init.h"

static unsigned long ls_dischargeTime;					// the discharge time in microseconds for the line sensors
static const unsigned ls_chargeTime = 10;				// it takes 10 microseconds to charge the line sensors
static const unsigned ls_defaultDischargeTime = 1000;	// default discharge time in microseconds
static volatile linesensor_t last_ls;					// cache last value read by line sensor

void StartChargingLS(void)
{
	// start charging
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
		| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0xff);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
		| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0xff);
}

void StartDischargingLS(void)
{
	// allow the sensors to discharge
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
		| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
}

linesensor_t ReadLS(void)
{
	linesensor_t ret = 0x00;
							  
	// read PB2-PB3 into bits 0-1 
	ret = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3) >> 2;
	// read PD2-PD7 into bits 2-7
	ret |= GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
		| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
															  
	return ret;
}

void LSIntHandler(void)
{
	static volatile tBoolean readyToRead = true;
	unsigned waitTime;

	// Mark interrupt as processed
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	if(readyToRead)					// finished waiting to discharge
	{					  
		last_ls = ReadLS();
		waitTime = g_ulTicksPerUs * ls_chargeTime;
		StartChargingLS();

	} else							// finished charging
	{						 
		waitTime = g_ulTicksPerUs * ls_dischargeTime;
		StartDischargingLS();
	}

	TimerLoadSet(TIMER0_BASE, TIMER_A, waitTime);
	readyToRead = !readyToRead;		// flip flag
}

// Summary: Initializes the appropriate GPIO
// Note:	Always call this function before any other sensor-related functions
//			Initializes dischargeTime to a default value of 1000 us
void InitializeLineSensor(void)
{
	// Enable the peripherals
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
		| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

	// default discharge time
	SetDischargeTime(ls_defaultDischargeTime);
}

// Summary:	Same as above, but sets up the line sensor for asynchronous operation.
// 			This means the line sensor will update automatically in the background
// Note:	Avoid mixing both normal and asynchronous operation
// Note:	Uses TIMER0. If you use asynchronous line sensing, you should not use TIMER0 elsewhere in your code;
//			If you use TIMER0 in your code, you should not use asynchronous line sensing.
void InitializeLineSensorAsync(void)
{
	// First, initialize the peripherals, etc.
	InitializeLineSensor();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Enable processor interrupts.
    IntMasterEnable();
    IntEnable(INT_TIMER0A);	

	// Then, set up the interrupt timers
	TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
	LSIntHandler();
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

// Summary: Sets the discharge time
// Parameters:
//		dischargeTime: the new discharge time 
//				constraints: 120 <= dischargeTime <= 2600
// Note:	This function applies to both normal and asynchronous operation
void SetDischargeTime(unsigned long dischargeTime)
{
	if(dischargeTime >= 120 && dischargeTime <= 2600)		// bounds check
		ls_dischargeTime = dischargeTime;
}

// Summary:	Reads the current line sensor values
// Note:	this method takes 10 us + dischargeTime to execute
// Returns:	a package of bits corresponding to the current readings from the line sensor
linesensor_t ReadLineSensor(void)
{					   
	StartChargingLS();								   
	// wait until fully charged (10 us)
	WaitUS(ls_chargeTime);

	StartDischargingLS();
	// wait 120 us <= threshold <= 2600 us											 
	WaitUS(ls_dischargeTime);

	return ReadLS();
}

// Summary:	Same as above, but immediately returns the last automatically-updated reading
// Note:	Uses TIMER0. If you use asynchronous line sensing, you should not use TIMER0 elsewhere in your code;
//			If you use TIMER0 in your code, you should not use asynchronous line sensing.
linesensor_t ReadLineSensorAsync(void)
{
	return last_ls;
}

