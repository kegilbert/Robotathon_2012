
#define LED_OFF 1
#define LED_ON  255

#include "driverlib/pwm.h"

typedef enum {
	LED_0 = PWM_OUT_0,
	LED_1 = PWM_OUT_1,
	LED_2 = PWM_OUT_2,
	LED_3 = PWM_OUT_3 
} led_t;

void LED_Init(void);
void LED_Set(led_t servo, unsigned char position);
