#ifndef SETTINGS_H
#define SETTINGS_H

#include "definitions.h"

//settings
/*
0x00 = autonomous
0x01 = nunchuck
*/
extern volatile unsigned char control_mode;

/*
0x00 = ackermann
0x01 = differential
*/
extern volatile unsigned char drive_type; 

/*
0x00 = microcontroller should manage
0x01 = computer should manage
*/
extern volatile unsigned char steering_pider;

/*
0x00 = microcontroller should manage
0x01 = computer should manage
*/
extern volatile unsigned char drive_pider;

/*
0x00 = microcontroller should read adcs and servos 
0x01 = microcontroller should listen for commands and output to servo ports
*/
extern volatile unsigned char paradigm;

extern volatile unsigned char battery_voltage; //can be anything, but should most likely be 12 or 24

#endif
