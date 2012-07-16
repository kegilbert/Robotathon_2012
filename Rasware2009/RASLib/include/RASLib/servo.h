// ***************************
// SERVO FUNCTIONS AND DEFINES
// ***************************
#ifndef SERVO_H
#define SERVO_H
#include "driverlib/pwm.h"

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

typedef unsigned char position_t;			// angle data type
typedef enum {
	SERVO_0 = PWM_OUT_0,
	SERVO_1 = PWM_OUT_1,
	SERVO_2 = PWM_OUT_2,
	SERVO_3 = PWM_OUT_3 } servo_t;		// servo data type

#define SERVO_MIN_POSITION		0
#define SERVO_NEUTRAL_POSITION	128
#define SERVO_MAX_POSITION		255

// Summary:	Initializes the appropriate PWMs for servo output
// Note:	Always call this function before any other servo-related functions
extern void InitializeServos(void);

// Summary: Sets the specified servo's position
// Parameters:
//		servo:	SERVO_0 SERVO_1 SERVO_2 SERVO_3
//		angle:	sets servo to the specified position
//				0 is turned counterclockwise, 128 is neutral, 255 is turned clockwise
extern void SetServoPosition(servo_t servo, position_t position);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // SERVO_H
