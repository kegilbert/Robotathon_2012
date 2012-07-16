#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#include "cb_compass.h"

#include "cb_i2c.h"

volatile unsigned char Compass_Values[2] = {0,0};
static volatile unsigned char index = 0;

static void nocallback(unsigned char *eh) {}
static void (*callback)(unsigned char *) = &nocallback;

void Compass_Init(void) {
	I2C_Init();
}

status_t CompassReadHandler(unsigned char val) {
	Compass_Values[index] = val;
	if (++index < 2) {
		return BUSY;
	} else {
		(*callback)((unsigned char *)Compass_Values);
		return I2C_Status;
	}
}

unsigned char * Compass_Read(void) {
	Compass_Background_Read(0);
	while (index < 2);
	return (unsigned char *)Compass_Values;
}

void Compass_Background_Read(void (*cb)(unsigned char *)) {
	cb = cb ? cb : &nocallback;
	index = 0;
	I2C_Background_Request(HMC6352,0x41,&CompassReadHandler);
}

