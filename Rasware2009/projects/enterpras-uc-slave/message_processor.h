/*
 * MessageProcessor.h
 * Provides functions that process any input sent to the microcontroller.
 * Author: Joshua James
 *
 */
 
#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

extern void handleMotorVoltageChangeRequest(char* request);
extern void handleVelocityRequest(char* request);
extern void handleHeartBeatRequest(char* request);
extern void processRequest(char* request);

#endif
