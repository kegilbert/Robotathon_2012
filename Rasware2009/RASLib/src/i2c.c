// ***************************
// I2C FUNCTIONS AND DEFINES
// ***************************

#include <stdlib.h>				// malloc & free
#include <stdarg.h>				// va_---- functions
#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"		// input/output over UART
#include "driverlib/i2c.h"
#include "RASLib/i2c.h"

static unsigned char i2c_buffer[32];
static unsigned short i2c_address;
static short i2c_buffer_index;

// Summary:	Starts the process of queueing data to send
// Parameters:
//		addr:	address to send data to
// Note:	Use this function when dealing with an unknown amount of characters to send
void I2CStart(unsigned short addr)
{
	//
	// Set address to send data to
	//  Left shift required by library
	//

	i2c_address = addr >> 1;
	
	// Initialize buffer
	i2c_buffer_index = 0;
}

// Summary:	Queue character to send
// Parameters:
//		data:	character to add to queue
// Note:	Use this function when dealing with an unknown amount of characters to send
void I2CAdd(short data)
{
	// Add data to queue
	i2c_buffer[i2c_buffer_index++] = data;
}

// Summary:	Send queued data
// Note:	Use this function when dealing with an unknown amount of characters to send
void I2CStop()
{
	unsigned char *data = i2c_buffer;	// Technically not needed, but cleans code a bit
	
	// Set address to read from
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, i2c_address, false);
	
	// Check to see if data will be sent
	if (i2c_buffer_index != 0)
		I2CMasterDataPut(I2C0_MASTER_BASE, *data);
	else
		return;
			
	// Send single piece of data if it's the only piece to send
	if (i2c_buffer_index == 1){
	    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
		while(I2CMasterBusy(I2C0_MASTER_BASE));
		
		return;
	}
	
	// Start sending consecutive data
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C0_MASTER_BASE));
	i2c_buffer_index--;
	data++;
	
	// Continue sending consecutive data
	while(i2c_buffer_index > 1){
		I2CMasterDataPut(I2C0_MASTER_BASE, *data);
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
		while(I2CMasterBusy(I2C0_MASTER_BASE));
		i2c_buffer_index--;
		data++;
	}
	
	// Send last piece of data
	I2CMasterDataPut(I2C0_MASTER_BASE, *data);
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
	while(I2CMasterBusy(I2C0_MASTER_BASE));
}

// Summary:	Sends 'num' number of characters to specified address
// Parameters:
//		addr:	address to send data to
//		num:	number of characters being sent
//		...:	characters to send, separated by commas
// Note:	Number of characters must be equal to 'num'
void I2CSend(unsigned short addr, int num, ...)
{
	// Make sure data is actually being sent
	if (num > 0 && num < 32){   // Max size of buffer
		// Allocate memory for data
		unsigned char *data = i2c_buffer;
		va_list args;
		int i=0;
		
		// Put characters to send in array
		va_start(args, num);
		for(; i<num; i++)
			data[i] = (unsigned char) va_arg(args, int);
		va_end(args);
	
		I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, addr >> 1, false);
		I2CMasterDataPut(I2C0_MASTER_BASE, *data);
		if (num == 1){
		    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
			while(I2CMasterBusy(I2C0_MASTER_BASE));
			return;
		}
		
		// Start sending consecutive data
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);
		while(I2CMasterBusy(I2C0_MASTER_BASE));
		num--;
		data++;
		
		// Continue sending consecutive data
		while(num > 1){
			I2CMasterDataPut(I2C0_MASTER_BASE, *data);
			I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
			while(I2CMasterBusy(I2C0_MASTER_BASE));
			num--;
			data++;
            UARTprintf("%d\n", num);
		}
		
		// Send last piece of data
		I2CMasterDataPut(I2C0_MASTER_BASE, *data);
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
		while(I2CMasterBusy(I2C0_MASTER_BASE));
	}
}

// Summary:	Recieve/Fetch data from specified address
// Parameters:
//		addr:	address to recieve data from
//		data:	pointer to memory location to save data
//		len:	number of cahracers that will be recieved
void I2CRecieve(unsigned short addr, unsigned char* data, unsigned int len)
{
    if (len < 1)	// Assume I2C Recieving will always return data
        return;
	
	// Set address to read from
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, addr >> 1, true);
	
	// Check to see if pointer is to an array
	if (len == 1){
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
		while(I2CMasterBusy(I2C0_MASTER_BASE));
		*data = I2CMasterDataGet(I2C0_MASTER_BASE);
		return;
	}
	
	// Begin reading consecutive data
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusy(I2C0_MASTER_BASE));
	*data = I2CMasterDataGet(I2C0_MASTER_BASE);
	len--;
	data++;
	
	// Continue reading consecutive data
	while(len > 1){
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    	while(I2CMasterBusy(I2C0_MASTER_BASE));
		*data = I2CMasterDataGet(I2C0_MASTER_BASE);
		len--;
		data++;
	}
	
	// Read last character of data	
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C0_MASTER_BASE));
	*data = I2CMasterDataGet(I2C0_MASTER_BASE);
}
