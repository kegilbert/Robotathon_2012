#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "RASLib/servo.h"

#include "RobzDemo.h"

void initServo(void) {
	InitializeServos();
	SetServoPosition(SERVO_0,0);
	SetServoPosition(SERVO_1,255);
}

void servoDemo(void) {
	unsigned char position = 0;
	unsigned char position2 = 255; 
	char newline = 13;
	char ch;
	

	UARTprintf("Press:\n  a-'up' 10\n  w-'up' 1\n  s-'down' 1\n");
	UARTprintf("  d-'down' 10\n  enter-quit\n");
	
		UARTprintf("position: %u\n  ",position);
		UARTprintf("position2: %u\n  ",position2);
		ch = getc();
		while(ch != newline) {
			if (ch == 'w'){
				position++;
				position2--;}
			else if (ch == 's'){
				position--;
				position2++;}	 
			else if (ch == 'a')
			{
				position+=10;
				position2-=10;
			}
			else if (ch == 'd')
			{
				position-=10;
				position2+=10;
			}
			SetServoPosition(SERVO_0, position); 
			SetServoPosition(SERVO_1, position2);
			SetServoPosition(SERVO_2, position);
			SetServoPosition(SERVO_3, position);
		
		UARTprintf("position: %u\n  ",position);
		UARTprintf("position2: %u\n  ",position2);	 
			ch = getc();
		}
				 
		UARTprintf("\n");
}
