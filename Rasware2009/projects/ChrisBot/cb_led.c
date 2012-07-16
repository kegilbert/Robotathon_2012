#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#include "cb_led.h"

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "RASLib/init.h"


static unsigned long ulServoPeriod;

void LED_Init(void) {
	
	ulServoPeriod = g_ulPWMTicksPerSecond / 120;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	// servos 0 & 1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	// servos 2 & 3
	   
  GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  // Set the PWM period to 120 Hz.
  PWMGenConfigure(PWM_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, ulServoPeriod);
  PWMGenConfigure(PWM_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM_BASE, PWM_GEN_1, ulServoPeriod);
	
	PWMOutputState(PWM_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT, true);

  PWMGenEnable(PWM_BASE, PWM_GEN_0);
  PWMGenEnable(PWM_BASE, PWM_GEN_1);

	// Default to 0
	LED_Set(LED_0, 0);
	LED_Set(LED_1, 0);
	LED_Set(LED_2, 0);
	LED_Set(LED_3, 0);
}


void LED_Set(led_t num, unsigned char position) {
	PWMPulseWidthSet(PWM_BASE, num, (ulServoPeriod*position)/255);
}

