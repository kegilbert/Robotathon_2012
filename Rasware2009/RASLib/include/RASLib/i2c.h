// ***************************
// I2C FUNCTIONS AND DEFINES
// ***************************
#ifndef I2C_H
#define I2C_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

// Summary:	Starts the process of queueing data to send
// Parameters:
//		addr:	address to send data to
// Note:	Use this function when dealing with an unknown amount of characters to send
extern void I2CStart(unsigned short addr);

// Summary:	Queue character to send
// Parameters:
//		data:	character to add to queue
// Note:	Use this function when dealing with an unknown amount of characters to send
extern void I2CAdd(short data);

// Summary:	Send queued data
// Note:	Use this function when dealing with an unknown amount of characters to send
extern void I2CStop(void);

// Summary:	Sends 'num' number of characters to specified address
// Parameters:
//		addr:	address to send data to
//		num:	number of characters being sent
//		...:	characters to send, separated by commas
// Note:	Number of characters must be equal to 'num'
extern void I2CSend(unsigned short addr, int num, ...);

// Summary:	Recieve/Fetch data from specified address
// Parameters:
//		addr:	address to recieve data from
//		data:	pointer to memory location to save data
//		len:	number of cahracers that will be recieved
extern void I2CRecieve(unsigned short addr, unsigned char* data, unsigned int len);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // I2C_H
