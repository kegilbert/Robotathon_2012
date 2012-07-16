/*
 * common.h
 * Holds common variables and locking functions
 * Author: Joshua James
 *
 */


#ifndef COMMON_H
#define COMMON_H

#include "definitions.h"

//==========TIMESTAMP==========
extern volatile unsigned long timestamp; //tick duration set in definitions.h

extern volatile unsigned char robotIsMoving;


//==========SENSOR DATA BUFFERS==========
//lock
extern volatile unsigned char sensor_lock;

//left motor encoder data
extern volatile SensorBuffer left_encoder_data;

//right motor encoder data
extern volatile SensorBuffer right_encoder_data;

//steering motor pot data
extern volatile SensorBuffer steering_pot_data;

//hokuyo lidar pot data
extern volatile SensorBuffer hokuyo_pot_data;

//==========CANON VALUES==========
//"canon" values are those currently accepted to be true by the robot
//lock
extern volatile unsigned char canon_lock;

//left motor stats
extern volatile signed long canon_left_velocity;
extern volatile signed long canon_left_position;

//right motor stats
extern volatile signed long canon_right_velocity;
extern volatile signed long canon_right_position;

//steering motor stats
extern volatile signed long canon_steering_velocity;
extern volatile signed long canon_steering_position;

//hokuyo lidar stats
extern volatile signed long canon_hokuyo_velocity;
extern volatile signed long canon_hokuyo_position;


//==========LOCKING FUNCTIONS==========
//sensor data buffer locking functions
extern void lockSensors(void);
extern void unlockSensors(void);
extern unsigned char sensorsLocked(void);

//canon data locking functions
extern void lockCanon(void);
extern void unlockCanon(void);
extern unsigned char canonLocked(void);

#endif
