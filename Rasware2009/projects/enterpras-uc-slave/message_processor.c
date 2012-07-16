/*
 * MessageProcessor.c
 * Provides functions that process any input sent to the microcontroller.
 * Author: Joshua James
 *
 */
 
 #include "UART_Functions.h"
 #include "message_processor.h"
 #include "definitions.h"
 
 /*
 void processRequest(char* request)
 {
	switch(request[CODE_START])
	{
		case 'S': handlePhysicalChangeRequest(request); 
				  break;
		case 'G': handleVelocityRequest(request);
				  break;
		case 'B': handleHeartBeatRequest(request);
				  break;
		default: break;
	}
}

void handlePhysicalChangeRequest(char* request)
{
	char length = extractString(request, LENGTH1, 1);
	char data = extractString(request, DATA, length);
	if(request[CODE_START + 1] == 'V' && request[CODE_START + 2] == 'L') //SVLM
	{
		if(request[DATA + length] == '_')	//Both SVLM and SVRM were requested
		{
			
		}
	}
	else if (request[CODE_START + 1] == 'V' && request[CODE_START + 2] == 'L') //SVRM
	{
	}
	else if(request[CODE_START + 1] == 'A' && request[CODE_START + 2] == 'S') //SASS
	{
		setSteeringAngle(data);
	}
	else if(request[CODE_START + 1] == 'A' && request[CODE_START + 2] == 'L') //SALS
	{
		setLidarAngle(data);
	}
	else
	{
		sendMessage("Invalid request\n");
	}
	sendMessage("{ACKN}\n");
}

void setMotorVoltage(char voltage)
{
	sendMessage("Someone asked to change the motors!\n");
}

void setSteeringAngle(char angle) //an angle of 0 should have a position of 128. an angle of -90 should have a position of 0. an angle of 90 should have a position of 256.
{
	unsigned char position = ((angle + 90) * 256) / 180; //convert angle to servo position. 
	SetServoPosition(SERVO_2, position);
}

void setLidarAngle(char angle)
{
	unsigned char position = ((angle + 90) * 256) / 180; //convert angle to servo position. 
	SetServoPosition(SERVO_3, position);
}

void handleVelocityRequest(char* request)
{
	char leftVelocityStringBytes = getNumberStringLength(leftVelocity);
	char rightVelocityStringBytes = getNumberStringLength(rightVelocity);
	char timeStringBytes = getNumberStringLength(time);
	sendMessage("{VELL: %d:%d_VELR:%d:%d_TIME:%d:%d}\n", leftVelocityStringBytes, leftVelocity, rightVelocityStringBytes, rightVelocity, timeStringBytes, time);
}

void handleHeartBeatRequest(char* request)
{
	sendMessage("Someone wants to keep the robot alive!\n");
}
*/
