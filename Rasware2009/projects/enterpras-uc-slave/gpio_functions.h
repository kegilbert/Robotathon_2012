/*
 * gpio_functions.h
 * Provides functions for control of devices attached to the gpio ports.
 * Author: Joshua James
 *
 */
 
#ifndef GPIO_FUNCTIONS_H
#define GPIO_FUNCTIONS_H

#include "definitions.h"

extern void initGPIO(void);
extern char controlSwitchPosition(void);
extern void turnOnWarningLight(void);
extern void turnOffWarningLight(void);
extern void toggleWarningLight(void);

#endif
