#include "inc/hw_types.h"		// tBoolean
#include "RASLib/motor.h"
#include "ADS7830.h"
#include "utils/uartstdio.h"
#include "driverlib/i2c.h"
#include "inc/hw_memmap.h"
#include "travel.h"
#include "clock.h"
#include "charging.h"
#include "RASLib/timer.h"



// Current location of robot
signed char currentCorner = TREE;
unsigned char currentFacing = false;
signed char motor_L = 0;
signed char motor_R = 0;

#define BACK_LEFT 0
#define FRONT_LEFT 1
#define FRONT 2
#define FRONT_RIGHT 3
#define BACK_RIGHT 4
#define BACK 5

#define LOWER 0
#define HIGHER 1

#define MOTOR_L_MAX 100
#define MOTOR_R_MAX 100

#define WALL_CONSTANT 1
#define TURN_CONSTANT 2
#define TIME_CONSTANT 1000





signed short piderror;	

signed char PID(unsigned char val,unsigned char goal,signed short Pk,signed short Dk) {
	signed char temp = piderror;
	piderror = (val-goal);
	return (signed char)((Pk*piderror + Dk*(piderror-temp))>>8);
}

//TODO experimentally determine these
const unsigned char bounds[2][6] = {
	{130, 80, 60,150, 60, 150},
	{ 70, 60, 40, 70, 40, 70},
};


void travelInit(void) {
	//motors
	InitializeMotors(true,false);
	ADS7830_Init();
}

void stop(void) {
	motor_L = 0;
	motor_R = 0;
}

void goForward(void) {
	motor_L = MOTOR_L_MAX;
	motor_R = MOTOR_R_MAX;
	piderror = 0;
}

void goForwardBlocking(void) {
	motor_L = 85;
	motor_R = 120;
	SetMotorPowers(motor_L, motor_R);
	piderror = 0;
}


void goBackward(void) {
	motor_L = -MOTOR_L_MAX;
	motor_R = -MOTOR_R_MAX;
	piderror = 0;
}

void goEngageCorner(signed char sourcetype) {
	if (sourcetype==ELECTRIC||sourcetype==FLAG) {
		till(true) { //replace false with switch that needs to be switched
		//approach wall
		}
	} else {
		till(tripped(FRONT,LOWER)) { //replace false with switch that needs to be switched
		//approach wall
		}
	}
}
#define IR_SENSOR_BIAS 0

int WallFollowPID(int in, int P , int I, int D, int divP, int divI, int divD){
	static int prev = 0;
	static int total = 0;
	int out;
	in += IR_SENSOR_BIAS;
	out = ((P * in)/divP) + ((I * total)/divI) + ((D * (in-prev))/divD);

	prev = in;
	total += in;

	return out;
}
int WallFollowPID2(int in, int P , int I, int D, int divP, int divI, int divD){
	static int prev = 0;
	static int total = 0;
	int out;
	
	out = ((P * in)/divP) + ((I * total)/divI) + ((D * (in-prev))/divD);

	prev = in;
	total += in;

	return out;
}

#define LEFT_MOTOR_OFFSET 3						   
#define LEFT_MOTOR_OFFSET_DIV 4

void SetMotorPowersBlocking(int left, int right){
	 if(left < -128)
	 	left = -128;
	 if(left > 127)
	 	left = 127;
	 if(right < -128)
	 	right = -128;
	 if(right > 127)
	 	right = 127;

	 SetMotorPowers( left*LEFT_MOTOR_OFFSET/LEFT_MOTOR_OFFSET_DIV , right );
}

enum {NEVER,CHARGING,TIMEOUT,EITHER,TRIP} StopWallFollow;
// inverse direction veers to the left - can look at again.
#define IDEAL_DISTANCE 100

#define P9v 2	//PD values for 9v
#define D9v	2
#define divP9v 1
#define divD9v 10

#define P12v 10	  //PD values for 12v 5 for both 
#define D12v 10
#define divP12v 1
#define divD12v 10


//use time for IR sensor value if using TRIP
void WallFollow(int mode, int time, int dir){
	 int startTime, diff, close;
	 switch(mode){
		 case NEVER:
		 while(1){
		 	 diff = WallFollowPID(  ADS7830_Values[FRONT_LEFT] -  ADS7830_Values[BACK_LEFT], P9v , 0 , D12v , divP12v, 1 , divD12v);
		 	 close = 0;//WallFollowPID2( IDEAL_DISTANCE - (ADS7830_Values[FRONT_LEFT] + ADS7830_Values[BACK_LEFT]) , 1 , 0 , 0, 1 , 0 , 0);
			 UARTprintf("Back Left: %d Front_Left: %d, close: %d, diff: %d\n",ADS7830_Values[BACK_LEFT], ADS7830_Values[FRONT_LEFT], close, diff);
			 SetMotorPowersBlocking( dir*(127 + diff + close) ,dir*(127 - diff - close));
		 }
		 //break;
		 case CHARGING:
		 while(!Charging()){
		 	 diff = WallFollowPID(  ADS7830_Values[FRONT_LEFT] -  ADS7830_Values[BACK_LEFT], P9v , 0 , D12v , divP12v, 1 , divD12v);
		 	 close = 0;//WallFollowPID2( IDEAL_DISTANCE - (ADS7830_Values[FRONT_LEFT] + ADS7830_Values[BACK_LEFT]) , 1 , 0 , 0, 1 , 0 , 0);
			 UARTprintf("Back Left: %d Front_Left: %d, close: %d, diff: %d\n",ADS7830_Values[BACK_LEFT], ADS7830_Values[FRONT_LEFT], close, diff);
			 SetMotorPowersBlocking( dir*(127 + diff + close) ,dir*(127 - diff - close));
		 }
		 break;
		 case TIMEOUT:
		 startTime = GetTimeMS();
		 while(GetTimeMS()<startTime+time){
		 	 diff = WallFollowPID(  ADS7830_Values[FRONT_LEFT] -  ADS7830_Values[BACK_LEFT], P9v , 0 , D12v , divP12v, 1 , divD12v);
		 	 close = 0;//WallFollowPID2( IDEAL_DISTANCE - (ADS7830_Values[FRONT_LEFT] + ADS7830_Values[BACK_LEFT]) , 1 , 0 , 0, 1 , 0 , 0);
			 UARTprintf("Back Left: %d Front_Left: %d, close: %d, diff: %d\n",ADS7830_Values[BACK_LEFT], ADS7830_Values[FRONT_LEFT], close, diff);
			 SetMotorPowersBlocking( dir*(127 + diff + close) ,dir*(127 - diff - close));
		 }
		 break;
		 case TRIP:
		 while(!tripped(time,LOWER)){
		 	 diff = WallFollowPID(  ADS7830_Values[FRONT_LEFT] -  ADS7830_Values[BACK_LEFT], P9v , 0 , D12v , divP12v, 1 , divD12v);
		 	 close = 0;//WallFollowPID2( IDEAL_DISTANCE - (ADS7830_Values[FRONT_LEFT] + ADS7830_Values[BACK_LEFT]) , 1 , 0 , 0, 1 , 0 , 0);
			 UARTprintf("Back Left: %d Front_Left: %d, close: %d, diff: %d\n",ADS7830_Values[BACK_LEFT], ADS7830_Values[FRONT_LEFT], close, diff);
			 SetMotorPowersBlocking( dir*(127 + diff + close) ,dir*(127 - diff - close));
		 }
		 break;

	 }
}

void BackOut(void){
	SetMotorPowers(-40,-128);
	Wait(500);
	SetMotorPowers(32,127);
	Wait(500);
	SetMotorPowers(127,127);
	Wait(300);
}


void goWall(void) {



	//temp for debug
	till(false) {
		if (tripped(FRONT_LEFT,HIGHER)) {
			motor_L = MOTOR_L_MAX-diff(BACK_LEFT,LOWER);
			motor_R = MOTOR_R_MAX-diff(FRONT_LEFT,LOWER);
		} else {
		 	motor_L = MOTOR_L_MAX;
			motor_R = MOTOR_R_MAX;
		}
		SetMotorPowers(motor_L, motor_R); //add by Stephen, unless it would be set elsewhere
	}

	
	/*signed char p = PID(ADS7830_Values[offset(FRONT_LEFT)],bounds[LOWER][offset(FRONT_LEFT)],100,0);
	if (motor_L+p > MOTOR_L_MAX) {
		motor_L = MOTOR_L_MAX;	
	} else if (motor_L+p < 0) {
		motor_L = 0;
	} else {
	   motor_L+=p;
	}*/
	//UARTprintf("diff : %d ",p);
	//UARTprintf("<%4d %4d>\n",motor_L,motor_R);
	//testSensors();
//	WaitUS(900000);
}


//rotates clockwise by degrees
void goRotate(signed char degrees) {
	SetMotorPowers(MOTOR_L_MAX,-MOTOR_R_MAX);
	WaitUS(degrees/TIME_CONSTANT);
	
}

//rotate to align with wall using IR sensors
void goAlignWall(char rightSensors, char goRight) {
	SetMotorPowers(MOTOR_L_MAX,-MOTOR_R_MAX);
	till(tripped(FRONT_LEFT,LOWER));
	till(tripped(BACK_LEFT ,LOWER));
}
						   
void gotoDest(signed char dest) {
	char flip = (dest==FAN && currentCorner!=FAN)||(dest!=FAN && currentCorner==FAN);
	signed char offdest;

	UARTprintf("At %d, goint to %d, fliping %d\n",currentCorner,dest,flip);

	if (dest<0 || dest==currentCorner) return;
	if (currentCorner==TREE) {
		till(tripped(FRONT,LOWER)) {UARTprintf("ah %d\n",ADS7830_Values[FRONT]);goForward();}
		UARTprintf("uhhhh");
		goAlignWall(false,true);
		WallFollow(TRIP,FRONT,1);
		currentCorner = ELECTRIC;
		currentFacing = false;
		if (dest != currentCorner) gotoDest(dest);
		return;
	}
	
	
	offdest = currentCorner - dest;
	if (offdest < 0) offdest = offdest+4;
	switch(offdest) {
		case 1:
			till(!tripped(FRONT_RIGHT,LOWER)) goBackward();
			goRotate(flip?-135:45); 
			currentFacing ^= flip;
			till(tripped(FRONT,LOWER)) goForward();
			goAlignWall(false,true);
			till(tripped(FRONT_RIGHT,LOWER)) goForward();
			break;
		case 2:
			till(tripped(BACK,LOWER)) goBackward();
			goRotate(45);
			goAlignWall(true,true);
			till(tripped(FRONT,LOWER)) goForward();
			goRotate(flip?-45:45);
			goAlignWall(flip,!flip);
			currentFacing ^= flip;
			break;
		case 3:
			till(tripped(BACK,HIGHER)) goBackward();
			goRotate(-45);
			till(tripped(BACK,LOWER)) goBackward();
			goRotate(flip?90:0);
			goAlignWall(flip,flip);
			currentFacing ^= flip;
			break;
	}
	
	goEngageCorner(dest);

	currentCorner = dest;
}

void testSensors(void) {
	//SetMotorPowers(motor_L,motor_R);
	ADS7830_Read();
	UARTprintf("[%3d %3d %3d %3d %3d %3d %3d %3d]\n",ADS7830_Values[0],ADS7830_Values[1],ADS7830_Values[2],ADS7830_Values[3],ADS7830_Values[4],ADS7830_Values[5],ADS7830_Values[6],ADS7830_Values[7]);
	WaitUS(100000);
}


