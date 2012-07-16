#include "ADS7830.h"

 //includes copied from I2C demo
#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"		// input/output over UART
#include "inc/hw_ints.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"		// input/output over UART
#include "driverlib/adc.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"

#include "RASLib/i2c.h"
#include "RASLib/timer.h"
//#include "../src/i2c.c"

unsigned char ADS7830_Values[8];
unsigned char index;

void ADS7830_Init() {
    // Init I2C Bus
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);	
	I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), true);
    I2CMasterEnable(I2C0_MASTER_BASE);

    // 0x84: 1 CH# 01 XX for request conversion. e.g 1 010 01 00 is for channel 2
	I2CMasterIntEnable(I2C0_MASTER_BASE);
	IntEnable(INT_I2C0);
	IntMasterEnable();
}

unsigned char I2CGet(void) {
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, ADS7830 >> 1, true);
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
	while(I2CMasterBusy(I2C0_MASTER_BASE));
	return I2CMasterDataGet(I2C0_MASTER_BASE);
}

void I2CRequest(unsigned char index) {
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, ADS7830 >> 1, false);
	I2CMasterDataPut(I2C0_MASTER_BASE, 0x84 | (index<<4));
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(I2C0_MASTER_BASE));
}



void I2CIntHandler(void) {
	ADS7830_Values[index] = I2CGet();
	I2CMasterIntClear(I2C0_MASTER_BASE);
	if (++index < 8) I2CRequest(index);
}


void ADS7830ReadNonBlocking(void) {
	I2CRequest(index = 0);
}

void ADS7830_Read(void) {
	I2CRequest(index = 0);

	while (index < 8);
}
