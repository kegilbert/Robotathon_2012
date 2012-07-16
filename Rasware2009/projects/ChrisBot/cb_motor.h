
//MOTOR_TYPE determines how the motor is controlled
//can be PWM, BINARY, RASLIB, or NONE
#define MOTOR_NONE   0
#define MOTOR_PWM    1
#define MOTOR_BINARY 2
#define MOTOR_RASLIB 3

#ifndef MOTOR_TYPE
#define MOTOR_TYPE MOTOR_RASLIB
#endif

#define MOTOR_FORWARD 127
#define MOTOR_STOP 0
#define MOTOR_BACKWARD -128

void Motor_Init(tBoolean,tBoolean);
void Motor_Set(signed char, signed char);
