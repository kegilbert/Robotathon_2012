/*
 * motor_functions.h
 * Provides functions for control of the motors.
 * Author: Joshua James, Nicu Stiurca, Cruz Monrreal II
 *
 */
 
#ifndef MOTOR_FUNCTIONS_H
#define MOTOR_FUNCTIONS_H

#include "definitions.h"

//extern void setMotorSpeeds(signed short left_speed, signed short right_speed, signed short angle);
extern void joyDrive(NunchuckData joy_data);
extern void pidSteeringServo(signed long desired_angle, signed long actual_angle);
extern void SetJaguarVoltage(unsigned char device, signed char voltage);

#endif
