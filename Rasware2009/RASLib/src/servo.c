// ***************************
// SERVO FUNCTIONS AND DEFINES
// ***************************

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

#include "RASLib/init.h"
#include "RASLib/servo.h"

static unsigned us600;
static unsigned us2400;

// Summary:	Initializes the appropriate PWMs for servo output
// Note:	Always call this function before any other servo-related functions
void InitializeServos(void)
{						
	unsigned long ulServoPeriod;

	//
	// Enable the peripherals used by the servos.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	// servos 0 & 1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	// servos 2 & 3
	   
    //
    // Set GPIO B0, B1, D0, and D1 as PWM pins. 
    // They are used to output the PWM0, PWM2, PWM3, and PWM3 signals.
    //
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Compute the PWM period based on the system clock.
    //
    ulServoPeriod = g_ulPWMTicksPerSecond / 50;

    //
    // Set the PWM period to 50 Hz = 20ms.
    //
    PWMGenConfigure(PWM_BASE, PWM_GEN_0,
                    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, ulServoPeriod);
    PWMGenConfigure(PWM_BASE, PWM_GEN_1,
                    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_1, ulServoPeriod);
    
    
    us600 = ulServoPeriod * 3 / 100;	// 20 ms * 3 / 100 = 600 us
    us2400 = us600 * 4;				// 600 us * 4 = 2400 us

    //
    // Enable the PWM0, PWM1, PWM2, and PWM3 output signals.
    //
    PWMOutputState(PWM_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT, true);

    //
    // Enable the PWM generator.
    //
    PWMGenEnable(PWM_BASE, PWM_GEN_0);
    PWMGenEnable(PWM_BASE, PWM_GEN_1);

	// Default to center
	SetServoPosition(SERVO_0, SERVO_NEUTRAL_POSITION);
	SetServoPosition(SERVO_1, SERVO_NEUTRAL_POSITION);
	SetServoPosition(SERVO_2, SERVO_NEUTRAL_POSITION);
	SetServoPosition(SERVO_3, SERVO_NEUTRAL_POSITION);
}

// Summary: Sets the specified servo's position
// Parameters:
//		servo:	SERVO_0 SERVO_1 SERVO_2 SERVO_3
//		angle:	sets servo to the specified position
//				0 is turned counterclockwise, 128 is neutral, 255 is turned clockwise
void SetServoPosition(servo_t servo, position_t position)
{
	unsigned short usPulseWidth;
	
	// map the position to a pulse width
	usPulseWidth = ((us2400-us600)*position)/256+us600;
		
	// set position
	PWMPulseWidthSet(PWM_BASE, servo, usPulseWidth);
}

