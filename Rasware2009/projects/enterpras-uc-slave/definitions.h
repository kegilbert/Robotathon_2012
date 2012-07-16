/*
 * definitions.h
 * Provides definitions and structs used throughout the code.
 * Author: Joshua James
 *
 */


#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "inc/hw_types.h"		// tBoolean

//==========COMM DEFINITIONS==========
//offset values in bytes
#define CODE_START					2
#define CODE_LENGTH					4
#define DATA_START					7

//code hash values
#define SVLM						'S'^'V'^'L'^'M'
#define SVRM						'S'^'V'^'R'^'M'
#define SVSM						'S'^'V'^'S'^'M'
#define SALS						'S'^'A'^'L'^'S'

//==========ROBOT CONTROl DEFINITIONS==========
//jurisdiction
#define MICROCONTROLLER				0
#define COMPUTER					1

//paradigm
#define ADC_SLAVE					0
#define SERVO_SLAVE					1

//control_modes
#define AUTONOMOUS 					0
#define NUNCHUCK 					16

//drive_types
#define ACKERMANN 					0
#define DIFFERENTIAL 				1

//==========ROBOT PHYSICAL CONSTANTS==========
#define MAX_TURN_ANGLE				60	//maximum turning angle of the front wheels in degrees (MUST BE LESS THAN 127)
#define CENTER_STEERING_POT_VALUE	647

//==========ROBOT PORT DEFINITIONS==========
//servo ports
#define LEFT_JAGUAR 				SERVO_0
#define RIGHT_JAGUAR 				SERVO_1
#define STEERING_SERVO				SERVO_2
#define LIDAR_SERVO					SERVO_3

#define DRIVE_JAGUAR 				SERVO_0 //if only one drive motor is used (as in the case of a mechanical differential), plug it in to servo port 0

//adc ports
#define POTENTIOMETER				0
#define HOKUYO						1

//encoder ports
#define LEFT_ENCODER				ENCODER_0
#define RIGHT_ENCODER				ENCODER_1

//GPIO ports 
#define CONTROL_TYPE_INPUT			GPIO_PIN_4 //(bank A)
#define WARNING_LIGHT				GPIO_PIN_7 //(bank D)

//==========TIMER DEFINITIONS==========
#define WATCHDOG_PERIOD 			50000 * g_ulTicksPerUs //robot will stop moving within 50ms of receiving no new command

//try to make SAMPLE_RATE / DATA_RATE an integer
#define TICK_RATE					1000	//frequency of timestamp ticks in HZ (1Khz = 1ms period)
#define SAMPLE_RATE					500		//TICK_RATE / SAMPLE_RATE MUST be an integer. 1000 is too fast for this.
#define	DATA_RATE					150 		//frequency of data messages to the computer in Hz(10Hz = 100ms period)
#define FLASH_RATE					4		//frequency of warning light toggle

//==========FILTER DEFINITIONS==========
#define FILTER_SIZE					SAMPLE_RATE/DATA_RATE

//==========I2C DEFINITIONS==========
#define codeSelect 		GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5) >> 2
#define I2C_TIMEOUT		10				//in timestamp ticks

//==========CHAR PROCESSING DEFINITIONS==========
#define isNum(x)		(x>='0' && x<='9')
#define isChar(x)		((x>='a' && x<='z') || (x>='A' && x<='Z'))
#define isHex(x)		((x>='a' && x<='f') || (x>='A' && x<='F')) || isNum(x)
#define charToByte(x)	(isNum(x) ? x-'0': ((x>='a' ? x-'a': x-'A')+10))

//==========WII NUNCHUCK DEFINITIONS==========
typedef struct NunchuckData 
{
	short x_position;
	short y_position;
	short x_acceleration;
	short y_acceleration;
	short z_acceleration;
	short c_button;
	short z_button;
	tBoolean valid;
} NunchuckData;

#define X_CENTER 55
#define Y_CENTER 128

//==========GENERAL CODES==========
#define MAINTAIN_STEERING_ANGLE -32768

//==========SENSOR DATA BUFFER STRUCT=============
typedef struct
{
   	signed long buffer[FILTER_SIZE];
	unsigned long index;
	unsigned long num_values;
} SensorBuffer;

#endif
