/*
 * adc_functions.h
 * Provides functions for control of devices attached to the adc ports.
 * Author: Joshua James
 *
 */
 
#ifndef ADC_FUNCTIONS_H
#define ADC_FUNCTIONS_H

#include "definitions.h"

extern void initADC(void);
extern unsigned long getCurrentSteeringAngle(void);
extern unsigned long getCurrentHokuyoAngle(void);
extern unsigned long getADC0(void);
extern unsigned long getADC1(void);
extern unsigned long getADC2(void);
extern unsigned long getADC3(void);

#endif
