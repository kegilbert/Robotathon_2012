#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "RASLib/encoder.h"

volatile static encoder_count_t enc0 = 0, enc1 = 0;
static signed dir0, dir1;		// direction to count (up/down) for each encoder

void InitializeEncoders(tBoolean invert0, tBoolean invert1)
{
	// enable and configure the GPIO pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	// enable the peripheral
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);	// enable the peripheral
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_6);	// configure pins as inputs
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);	// configure pins as inputs
	
	// enable and configure the interrupts
	IntEnable(INT_GPIOB);					// enable interrupts for the periph
	IntEnable(INT_GPIOC);					// enable interrupts for the periph
	GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_6, GPIO_BOTH_EDGES);	// configure the interrupts
	GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6, GPIO_BOTH_EDGES);	// configure the interrupts
	GPIOPinIntEnable(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_6);	// enable the interrupt for the pins
	GPIOPinIntEnable(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);	// enable the interrupt for the pins
	
	dir0 = invert0 ? -1 : 1;
	dir1 = invert1 ? -1 : 1;
}

void GetEncoderCounts(encoder_count_t *pEncCount0, encoder_count_t *pEncCount1)
{
	*pEncCount0 = enc0;
	*pEncCount1 = enc1;
}

encoder_count_t GetEncoderCount(encoder_t encoder)
{
	switch(encoder)
	{
	case ENCODER_0:
		return enc0;
	case ENCODER_1:
		return enc1;
	default:
		return 0;
	}
}

void PresetEncoderCounts(encoder_count_t new_count0, encoder_count_t new_count1)
{
	enc0 = new_count0;
	enc1 = new_count1;
}

void PresetEncoderCount(encoder_t encoder, encoder_count_t new_count)
{
	switch(encoder)
	{
	case ENCODER_0:
		enc0 = new_count;
		break;
	case ENCODER_1:
		enc1 = new_count;
		break;
	}
}

void EncoderInterruptHandler(void)
{
	int temp = 0;
	unsigned char pin, pinA, pinB;
	unsigned port;
	signed dir;
	volatile encoder_count_t *p_encCount;
	
	if((GPIOPinIntStatus(port=GPIO_PORTC_BASE, false) & (pin=GPIO_PIN_5)))
	{
		// encoder 0 fired!
		pinA = GPIO_PIN_5;
		pinB = GPIO_PIN_4;
		dir = dir0;
		p_encCount = &enc0;
	}
	else if(GPIOPinIntStatus(port=GPIO_PORTB_BASE, false) & (pin=GPIO_PIN_4))
	{
		// encoder 0 fired!
		pinA = GPIO_PIN_5;
		pinB = GPIO_PIN_4;
		dir = -dir0;
		p_encCount = &enc0;
	}
	else if(GPIOPinIntStatus(port=GPIO_PORTC_BASE, false) & (pin=GPIO_PIN_6))
	{
		// encoder 1 fired!
		pinA = GPIO_PIN_6;
		pinB = GPIO_PIN_6;
		dir = dir1;
		p_encCount = &enc1;
	} 
	else if (GPIOPinIntStatus(port=GPIO_PORTB_BASE, false) & (pin=GPIO_PIN_6))
	{
		// encoder 1 fired!
		pinA = GPIO_PIN_6;
		pinB = GPIO_PIN_6;
		dir = -dir1;
		p_encCount = &enc1;
	} else return;
	GPIOPinIntClear(port, pin);
	
	if(GPIOPinRead(GPIO_PORTC_BASE, pinA))	++temp;	// channel A
	if(GPIOPinRead(GPIO_PORTB_BASE, pinB))	++temp;	// channel B

	switch(temp)
	{
	case 0:	// both channels == 0
	case 2:	// both channels == 1
		// channel A == channel B
		*p_encCount += dir;	// --> moving forward
		break;
	case 1:	// one channel == 1 and the other == 0
		// channel A != channel B
		*p_encCount -= dir;	// --> moving backward
		break;
	}
}
	
