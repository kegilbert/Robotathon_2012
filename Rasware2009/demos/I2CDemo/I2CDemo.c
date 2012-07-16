#include <stdlib.h>				// malloc & free
#include <stdarg.h>				// va_---- functions
#include <string.h>				// String functions
#include <math.h>
#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"		// input/output over UART
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"		// input/output over UART
#include "driverlib/adc.h"
#include "driverlib/i2c.h"
#include "RASLib/init.h"
#include "RASLib/timer.h"		// input/output over UART
#include "RASLib/servo.h"
#include "RASLib/uart.h"
#include "RASLib/motor.h"
#include "RASLib/encoder.h"
#include "RASLib/i2c.h"
#include "../src/i2c.c"

#define codeSelect GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5) >> 2
#define isNum(x)		(x>='0' && x<='9')
#define isChar(x)		((x>='a' && x<='z') || (x>='A' && x<='Z'))
#define isHex(x)		((x>='a' && x<='f') || (x>='A' && x<='F')) || isNum(x)
#define charToByte(x)	(isNum(x) ? x-'0': ((x>='a' ? x-'a': x-'A')+10))

void InitializeI2C(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);
}

void init_nunchuck(){

    UARTprintf("Initializing Wireless Nunchuck\n\n");   
    while(1){
        I2CSend(0x52<<1, 2, 0xF0, 0x55);
        if (I2CMasterErr(I2C0_MASTER_BASE) == I2C_MASTER_ERR_NONE){
            Wait(1);
            I2CSend(0x52<<1, 2, 0xFB, 0x00);
            if (I2CMasterErr(I2C0_MASTER_BASE) == I2C_MASTER_ERR_NONE)
                break;
        }
        
        Wait(10);
    }
}

int main(void){
    char i;
	unsigned char data[16];
    short wiichuck[7], xinit=0, yinit=0, l_vel, r_vel;
    int xpow, ypow;

 	LockoutProtection();
	InitializeMCU();
	InitializeUART();
    InitializeI2C();
    
	InitializeServos();
    SetServoPosition(SERVO_0, 140);
    
	InitializeMotors(true, false);
	InitializeEncoders(true, false);
    
//	UARTprintf("Initializing Nunchuck\n\n");
//	I2CSend(0x52<<1, 2, 0x40, 0x00);
//  Wait(25);
    
    init_nunchuck();
    
    // Wireless Nunchucks Zero @ 128
    xinit = yinit = 128;
        
	while(1){
		//Start Recalculating Values
        Wait(1);
		I2CSend(0x52<<1, 1, 0x00);
        Wait(1);   
		I2CSend(0x52<<1, 1, 0x00);
        Wait(1);     
		I2CSend(0x52<<1, 1, 0x00);
        
        if (I2CMasterErr(I2C0_MASTER_BASE) != I2C_MASTER_ERR_NONE){
            UARTprintf("Send Zero Error:\n");
            switch(I2CMasterErr(I2C0_MASTER_BASE)){
                case I2C_MASTER_ERR_ADDR_ACK:
                    UARTprintf(" I2C_MASTER_ERR_ADDR_ACK\n");
                    break;
                case I2C_MASTER_ERR_DATA_ACK:
                    UARTprintf(" I2C_MASTER_ERR_DATA_ACK\n");
                    break;
                case I2C_MASTER_ERR_ARB_LOST:
                    UARTprintf(" I2C_MASTER_ERR_ARB_LOST\n");
                    break;
                default:
                    UARTprintf("WTF: %d\n", I2CMasterErr(I2C0_MASTER_BASE));
            }
            
            // Reinitialize Nunchuck on error
            init_nunchuck();
        }else{
            Wait(1);
            I2CRecieve(0x52<<1, data, 6);   // Nunchuck data is 6 bytes, but for whatever reason, MEMOREX Wireless Nunchuck wants to send 8...
            
            if (I2CMasterErr(I2C0_MASTER_BASE) != I2C_MASTER_ERR_NONE){
                UARTprintf("Send Zero Error:\n");
                switch(I2CMasterErr(I2C0_MASTER_BASE)){
                    case I2C_MASTER_ERR_ADDR_ACK:
                        UARTprintf(" I2C_MASTER_ERR_ADDR_ACK\n");
                        break;
                    case I2C_MASTER_ERR_DATA_ACK:
                        UARTprintf(" I2C_MASTER_ERR_DATA_ACK\n");
                        break;
                    case I2C_MASTER_ERR_ARB_LOST:
                        UARTprintf(" I2C_MASTER_ERR_ARB_LOST\n");
                        break;
                }
                
                // Reinitialize Nunchuck on error
                init_nunchuck();
            }else{
                //for(i=0; i<6; i++)
                //    data[i] = (data[i] ^ 0x17) + 0x17;  // Nintendo decided to encrypt thir data...
        
        		// Save Joystick Data
        		wiichuck[0] = data[1];                                          // X Axis Joystick
        	    wiichuck[1] = data[0];                                          // Y Axis Joystick
        		wiichuck[2] = (((unsigned short) data[2]) << 2) + (((unsigned short) data[5]) & (3<<2));    // X Axis Accel
        		wiichuck[3] = (((unsigned short) data[3]) << 2) + (((unsigned short) data[5]) & (3<<4));    // Y Axis Accel
        		wiichuck[4] = (((unsigned short) data[4]) << 2) + (((unsigned short) data[5]) & (3<<6));    // Z Axis Accel
        		wiichuck[5] = data[5] & (1 << 1) ? 0 : 1;                              //'C' Button 
        		wiichuck[6] = data[5] & (1 << 0) ? 0 : 1;                              //'Z' Button
            
            //if (xinit == 0 && yinit == 0){
            //    xinit = wiichuck[0]-127;
            //    yinit = wiichuck[1]-127;
           //}else{
                xpow = (wiichuck[0]-xinit)/2;
                ypow = (wiichuck[1]-yinit)/2;
                l_vel = (xpow - ypow)*2;
                r_vel = (xpow + ypow)*2;
                
                l_vel = l_vel > 127 ? 127 : l_vel;
                r_vel = r_vel > 127 ? 127 : r_vel;
                l_vel = l_vel < -127 ? -127 : l_vel;
                r_vel = r_vel < -127 ? -127 : r_vel;
                
                //UARTprintf("X: %d\tY: %d\n", xpow*2, ypow*2);
                SetMotorPowers(l_vel / (wiichuck[5]==0 ? 2 : 1), r_vel / (wiichuck[5]==0 ? 2 : 1));
                UARTprintf("Motor L: %d\tMotor R: %d\n", l_vel, r_vel);
                SetServoPosition(SERVO_0, wiichuck[6]==1 ? 255 : 140);
                UARTprintf("Nunchuck Data:\n");
                for(i=0; i<7; i++){
                    UARTprintf(" %d\n", wiichuck[i]);
                }NL;
                
                Wait(100);
            }
        }
	}
}

/*
void readline(char* line, int len){
	int i=0;
	char in;
	
	for(; i<len-1; i++){
		in = UARTgetc();
		
		if (in == 0x1B){			// Excape Key
			line[0] = '\0';
			return;
		}
		
		if (in == 0x7f && i == 0)	// Backspace Key (Begining of line)
			i--;
		else{
			UARTprintf("%c", in);
		
			if (in == 0x7F && i!=0)	// Backspace Key (Middle of line)
				i-=2;
			else
				line[i] = in;
			
			if (in == 0x0D){		// Return Key
				line[i] = '\0';
				return;
			}
		}
	}
	line[len] = '\0';
}

int main(void){
	char line[64], cmd[64], addr=0x00;
	int i, ndx, read;
	
	//unsigned short bearing=0;
    unsigned char data[16];
		
	LockoutProtection();
	InitializeMCU();
	InitializeUART();
    InitializeI2C();
	
	UARTprintf("\n\nI2C DEMO\n");
	UARTprintf(" 0    Scan for Devices\n");
	UARTprintf(" 1    Select Address\n");
	UARTprintf(" 'A'  Send Character\n");
	UARTprintf(" 0x00 Send Hex\n");
	UARTprintf(" r:#  Read # of bytes\n");
			
	while(1){
        UARTprintf("[0x%x%x]: ", (addr&0xF0)>>4, addr&0x0F);
		readline(line, sizeof line);
		UARTprintf("\n");
	
		if (line[0] == '0' && line[1]=='\0'){
			UARTprintf("Searching I2C Address Space\n");
			
			ndx=0;
			for(i=0; i<127; i++){
				I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, i, false);
				I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
				while(I2CMasterBusy(I2C0_MASTER_BASE));
				if(I2CMasterErr(I2C0_MASTER_BASE) == I2C_MASTER_ERR_NONE)
				    UARTprintf("0x%2X ", i<<1);
			}
			UARTprintf("\n");
		}else if (line[0] == '1' && line[1]==' ' && line[2]=='0' && line[3]=='x' && (isNum(line[4]) || isChar(line[4])) && (isNum(line[5]) || isChar(line[5]))){
			if (!(isHex(line[4]) && isHex(line[5])))
				UARTprintf(" Malformed Address: 0x%c%c\n", line[4], line[5]);
			else
				addr=charToByte(line[4])<<4 | charToByte(line[5]);
		}else if (line[0] == '\0'){
			UARTprintf("\n\nI2C DEMO\n");
			UARTprintf(" 0    Scan for Devices\n");
			UARTprintf(" 1    Select Address\n");
			UARTprintf(" 'A'  Send Character\n");
			UARTprintf(" 0x00 Send Hex\n");
			UARTprintf(" r:#  Read # of bytes\n");
		}else if (addr != 0){
			ndx=0;
			read=0;
			I2CStart(addr);
			
			for(i=0; i < 64; i++){
				if (line[i] == ' ' || line[i] == '\0'){
					cmd[ndx] = '\0';
										
					if (ndx==3 && cmd[0]=='\'' && isChar(cmd[1]) && cmd[2]=='\''){
						// Sending Char
						I2CAdd(cmd[1]);
					}else if (ndx==4 && cmd[0]=='0' && cmd[1]=='x' && (isNum(cmd[2]) || isChar(cmd[2])) && (isNum(cmd[3]) || isChar(cmd[3]))){
						// Sending Hex
						if (!(isHex(cmd[2]) && isHex(cmd[3]))){
							UARTprintf(" Malformed Hex: %s\n", cmd);
							break;	// Break out
						}
						
						I2CAdd(charToByte(cmd[2])<<4 | charToByte(cmd[3]));
					}else if ((ndx==3 || (cmd[3]=='c' && ndx==4)) && (cmd[0]=='r' || cmd[0]=='R') && cmd[1]==':' && (isNum(cmd[2]) && cmd[2]!='0')){
						// Reading Byte(s)
						read=charToByte(cmd[2]);
						I2CStop();
						
						if (cmd[3]=='c'){
							while(!UARTCharsAvail(UART0_BASE)){
								I2CRecieve(addr, data, read);
								UARTprintf(" Data Recieved: ");
								for(ndx=0; ndx<read; ndx++)
									UARTprintf("0x%x%x ", (data[ndx]&0xF0)>>4, data[ndx]&0x0F);
								UARTprintf("\n");
								SysCtlDelay(SysCtlClockGet() / 30);
							}
							Wait(100);
						}else{
							I2CRecieve(addr, data, read);
							UARTprintf(" Data Recieved: ");
							for(ndx=0; ndx<read; ndx++)
									UARTprintf("0x%x%x ", (data[ndx]&0xF0)>>4, data[ndx]&0x0F);
							UARTprintf("\n");
						}
						
						break;		// Read IS last command
					}else{
							UARTprintf(" Invalid Command: %X\n%s\n%s\n", i, cmd, line);
							break;	// Break out
					}
					
					if (line[i] == '\0'){
						I2CStop();
						break;
					}
					
					ndx=0;
				 }else
					cmd[ndx++] = line[i];
			}
		}
	}
	
	
	UARTprintf("\n------------\n");
	
	I2CSend(0x42, 2, 'r', 0x06);
	I2CRecieve(0x42, data, 1);
	
	UARTprintf("# of Samples: %X\n", data[0]);
	while(1){	   
		bearing=0;
		
		I2CSend(0x42, 1, 'A');
		Wait(1);

        I2CRecieve(0x42, data, 2);
		
        bearing = data[0]<<8 | data[1]; 
        //UARTprintf("Bearing: %u.%u\n", bearing/10, bearing%10);
		UARTprintf("%X %X\n", data[0], data[1]);
		
		Wait(200);
	}
}
   */
