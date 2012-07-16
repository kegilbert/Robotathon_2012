/*
 * common.c
 * Holds common variables and locking functions
 * Author: Joshua James
 *
 */
 
#include "common.h"
#include "definitions.h" 
 
//==========TIMESTAMP==========
volatile unsigned long timestamp; //tick duration set in definitions.h

volatile unsigned char robotIsMoving;


//==========SENSOR DATA BUFFERS==========
//lock
volatile unsigned char sensor_lock;

//left motor encoder data
volatile SensorBuffer left_encoder_data;

//right motor encoder data
volatile SensorBuffer right_encoder_data;

//steering motor pot data
volatile SensorBuffer steering_pot_data;

//hokuyo lidar pot data
volatile SensorBuffer hokuyo_pot_data;

//==========CANON VALUES==========
//"canon" values are those currently accepted to be true by the robot
//lock
volatile unsigned char canon_lock;

//left motor stats
volatile signed long canon_left_velocity;
volatile signed long canon_left_position;

//right motor stats
volatile signed long canon_right_velocity;
volatile signed long canon_right_position;

//steering motor stats
volatile signed long canon_steering_velocity;
volatile signed long canon_steering_position;

//hokuyo lidar stats
volatile signed long canon_hokuyo_velocity;
volatile signed long canon_hokuyo_position;


//==========LOCKING FUNCTIONS==========
//sensor data buffer locking functions
void lockSensors(void)
{
	sensor_lock = 1;
}

void unlockSensors(void)
{
	sensor_lock = 0;
}

unsigned char sensorsLocked(void)
{
	return sensor_lock;
}

//canon data locking functions
void lockCanon(void)
{
	canon_lock = 1;
}

void unlockCanon(void)
{
	canon_lock = 0;
}

unsigned char canonLocked(void)
{
	return canon_lock;
}
