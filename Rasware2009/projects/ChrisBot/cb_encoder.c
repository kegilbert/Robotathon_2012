#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#include "cb_encoder.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

volatile signed long Encoder_Values[2];

static signed dir0,dir1;
static signed long stopval0,stopval1;

static void nocallback(void) {}
static void (*callback0)(void) = &nocallback;
static void (*callback1)(void) = &nocallback;
static void (*constantcallback)(signed long *) = (void (*)(signed long *))&nocallback;

#define ENCODE(enc,op,pina,pinb) {				\
	if((Encoder_Values[enc] op##=						\
	((!GPIOPinRead(GPIO_PORTC_BASE,pina)^  	\
		!GPIOPinRead(GPIO_PORTB_BASE,pinb))		\
			 ? -dir##enc : dir##enc))						\
					==stopval##enc)									\
			(*callback##enc)();									\
}

void EncoderIntHandler(void) {
	
	if (GPIOPinIntStatus(GPIO_PORTC_BASE,false) & GPIO_PIN_5) {
		GPIOPinIntClear(GPIO_PORTC_BASE, GPIO_PIN_5);
		ENCODE(0,+,GPIO_PIN_5,GPIO_PIN_4);
		
	} else if(GPIOPinIntStatus(GPIO_PORTB_BASE,false) & GPIO_PIN_4) {
		GPIOPinIntClear(GPIO_PORTB_BASE, GPIO_PIN_4);
		ENCODE(0,-,GPIO_PIN_5,GPIO_PIN_4);
		
	} else if(GPIOPinIntStatus(GPIO_PORTC_BASE,false) & GPIO_PIN_6) {
		GPIOPinIntClear(GPIO_PORTC_BASE, GPIO_PIN_6);
		ENCODE(1,+,GPIO_PIN_6,GPIO_PIN_6);
		
	} else if(GPIOPinIntStatus(GPIO_PORTB_BASE,false) & GPIO_PIN_6) {
		GPIOPinIntClear(GPIO_PORTB_BASE, GPIO_PIN_6);
		ENCODE(1,-,GPIO_PIN_6,GPIO_PIN_6);
		
	}
	
	(*constantcallback)((signed long *)Encoder_Values);
}

void Encoder_Init(tBoolean inv0, tBoolean inv1) {
	dir0 = inv0 ? -1 : 1;
	dir1 = inv1 ? -1 : 1;
	
	// enable and configure the GPIO pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_6);
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);
	
	// enable and configure the interrupts
	GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_6, GPIO_BOTH_EDGES);
	GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6, GPIO_BOTH_EDGES);
	GPIOPinIntEnable(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_6);
	GPIOPinIntEnable(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);
	IntEnable(INT_GPIOB);
	IntEnable(INT_GPIOC);
}

signed long * Encoder_Read(void) {
	return (signed long *)Encoder_Values;
}

void Encoder_Background_Read(void (*cb)(signed long *)) {
	constantcallback = cb ? cb : (void (*)(signed long *))nocallback;
}

void Encoder_Preset(signed long val0, signed long val1) {
	Encoder_Values[0] = val0;
	Encoder_Values[1] = val1;
}

void Encoder_Callback(unsigned char enc, signed long stopval, void (*cb)(void)) {
	if (enc) {
		stopval1 = stopval;
		callback1 = cb ? cb : &nocallback;
	} else {
		stopval0 = stopval;
		callback0 = cb ? cb : &nocallback;
	}
}
