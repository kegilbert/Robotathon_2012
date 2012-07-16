#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#include "cb_motor.h"

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

#if MOTOR_TYPE == MOTOR_RASLIB
#include "RASLib/motor.h"
#endif

#if MOTOR_TYPE == MOTOR_BINARY
static tBoolean inv0,inv1;
#endif

void Motor_Init(tBoolean i0, tBoolean i1) {

#if MOTOR_TYPE == MOTOR_RASLIB
	InitializeMotors(i0,i1);
#endif
	
#if (MOTOR_TYPE == MOTOR_BINARY || MOTOR_TYPE == MOTOR_PWM)
	
#if MOTOR_TYPE == MOTOR_BINARY
	inv0 = i0;
	inv1 = i1;
#endif
	
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	
#if MOTOR_TYPE == MOTOR_PWM
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
		GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
		
		PWMGenConfigure(PWM_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_2, 256);
		
		PWMOutputInvert(PWM_BASE, PWM_OUT_4_BIT, i0);
		PWMOutputInvert(PWM_BASE, PWM_OUT_5_BIT, i1);
		
		PWMOutputState(PWM_BASE, PWM_OUT_4_BIT | PWM_OUT_5_BIT, true);
    PWMGenEnable(PWM_BASE, PWM_GEN_2);
#else
		GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
#endif
	
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);

	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
#endif
	Motor_Set(0,0);
}

void Motor_Set(signed char m0, signed char m1) {
#if MOTOR_TYPE == MOTOR_RASLIB
		SetMotorPowers(m0,m1);
#elif MOTOR_TYPE == MOTOR_PWM
		PWMPulseWidthSet(PWM_BASE, PWM_OUT_4, m0+128);
		PWMPulseWidthSet(PWM_BASE, PWM_OUT_5, m1+128);
#elif MOTOR_TYPE == MOTOR_BINARY
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, m0? 0:GPIO_PIN_7);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, m1? 0:GPIO_PIN_5);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1, 
			((m0<0)^inv0 ? 0:GPIO_PIN_0) | ((m1<0)^inv1 ? 0:GPIO_PIN_1));
#endif
}

