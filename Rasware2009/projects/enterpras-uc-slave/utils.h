/*
 * utils.h
 * Provides general utilities for data manipulation.
 * Author: Joshua James
 *
 */
 
#ifndef UTILS_H
#define UTILS_H

#include "definitions.h"
#include "RASLib/servo.h"

extern signed char turnDirection(signed char angle1, signed char angle2);
extern char angleBetween(signed char angle1, signed char angle2);
extern unsigned char makeChecksum(char* data, unsigned long buffer_size);
extern unsigned char checksumIsCorrect(char* data, unsigned long buffer_size);
extern void pidGraph(void);
extern void push(signed long value, SensorBuffer* sensorBuffer);
extern void testServoPort(servo_t device);

#endif
