#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#include "cb_travel.h"

#include "cb_encoder.h"
#include "cb_motor.h"

static tBoolean usemotors;
static signed char speed;

void ForwardHandler(signed long * data) {
	if (usemotors) 
		Motor_Set(
			data[0]<=data[1] ? speed : speed-(16*(data[0]-data[1])), 
			data[1]<=data[0] ? speed : speed-(16*(data[1]-data[0]))
		);
}

void Travel_Init(tBoolean motors) {
	usemotors = motors;
	if (usemotors) Motor_Init(false,true);
	Encoder_Init(true,false);
}

void Travel_Go(signed char s) {
	speed = s;
	if (s >= 0) Encoder_Background_Read(&ForwardHandler);
	if (usemotors) Motor_Set(speed,speed);
}

void Travel_Stop(void) {
	Encoder_Background_Read(0);
	if (usemotors) Motor_Set(0,0);
}


void Travel_Offset(signed char off) {
	Encoder_Values[0] -= off;
}

void Travel_Spin(signed char speed) {
	Encoder_Background_Read(0);
	if (usemotors) Motor_Set(-speed,speed);
}
