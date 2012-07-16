// ***************************
// MOTOR FUNCTIONS AND DEFINES
// ***************************

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

#include "RASLib/init.h"
#include "RASLib/motor.h"

static unsigned long s_ulMotorPeriod;

// Summary:	Initializes the appropriate PWMs for motor output
// Note:	Always call this function before any other motor-related functions
void InitializeMotors(tBoolean invert0, tBoolean invert1)
{
    //
    // Enable the peripherals used by the motors.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);	// PWM4 & PWM5 are on port E
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);	// breaking pin is on port C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	// breaking pin is on port B

    //
    // Set GPIO E0 and E1 as PWM pins.  They are used to output the PWM4 and
    // PWM5 signals.
    //
    GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	// Configure breaking pins PC7 and PB5
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);

    //
    // Compute the PWM period based on the system clock, 2kHz.
    //
    s_ulMotorPeriod = 256 + 8;	// we want granularity of 256 steps
								// plus a dead-band of 4 on either side
    PWMGenConfigure(PWM_BASE, PWM_GEN_2,
                    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_2, s_ulMotorPeriod);
    
    //
    // if we need to invert the clockwise and counter-clockwise directions,
    // we can just invert the PWM signal because of how the H-bridges operate
    //
	PWMOutputInvert(PWM_BASE, PWM_OUT_4_BIT, invert0);
	PWMOutputInvert(PWM_BASE, PWM_OUT_5_BIT, invert1);
	
	//
    // Enable the PWM4 and PWM5 output signals.
    //
    PWMOutputState(PWM_BASE, PWM_OUT_4_BIT | PWM_OUT_5_BIT, true);

    //
    // Enable the PWM generator.
    //
    PWMGenEnable(PWM_BASE, PWM_GEN_2);

	//
	// Initialize motors stopped
	//
	SetMotorPowers(0, 0);
	
	//
	// Signal the H-bridges to turn the motors on
	//
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
}

// Summary: Sets the specified motor's power
// Parameters:
//		motor:		MOTOR_0 or MOTOR_1
//		power:		sets motor to the specified power.
//					0 is neutral, 127 is full forward, -128 is full backward
void SetMotorPower(motor_t motor, power_t power)
{
	unsigned short usPulseWidth;
	
	// map the power to a pulse width
	usPulseWidth = power + 128 + 4;
		
	// set power
	PWMPulseWidthSet(PWM_BASE, motor, usPulseWidth);
}

// Summary: Sets both motor powers simultaneously
// Parameters:
//		power0:		desired power output for MOTOR_0
//		power1:		desired power output for MOTOR_1
void SetMotorPowers(power_t power0, power_t power1)
{
	SetMotorPower(MOTOR_0, power0);
	SetMotorPower(MOTOR_1, power1);
}

