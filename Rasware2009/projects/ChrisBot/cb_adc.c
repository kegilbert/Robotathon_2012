#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#include "cb_adc.h"

#include "cb_i2c.h"

volatile unsigned char ADC_Values[8] = {0,0,0,0,0,0,0,0};
static volatile unsigned char index;
static tBoolean single;

static void nocallback() {}
static void (*callback)() = &nocallback;

void ADC_Init(void) {
	I2C_Init();
}

status_t ADCReadHandler(unsigned char val) {
	ADC_Values[index] = val;
	if (single) {
		(*callback)(val);
	} else {
		if (++index < 8) {
			I2C_Background_Request(ADS7830,0x84|(index<<4),&ADCReadHandler);
		} else {
			(*callback)((unsigned char *)ADC_Values);
		}
	}
	
	return I2C_Status;
}

unsigned char ADC_Single_Read(unsigned char i) {
	return ADC_Values[i] = I2C_Request(ADS7830,0x84|(i<<4));
}

void ADC_Single_Background_Read(unsigned char i,void (*cb)(unsigned char)) {
	callback = cb ? (void (*)())cb : &nocallback;
	single = true;
	I2C_Background_Request(ADS7830,0x84|((index=i)<<4),&ADCReadHandler);
}

unsigned char * ADC_Read(void) {
	ADC_Background_Read(0);
	while (index < 8);
	return (unsigned char *)ADC_Values;
}

void ADC_Background_Read(void (*cb)(unsigned char *)) {
	callback = cb ? (void (*)())cb : &nocallback;
	single = false;
	I2C_Background_Request(ADS7830,0x84|((index=0)<<4),&ADCReadHandler);
}
