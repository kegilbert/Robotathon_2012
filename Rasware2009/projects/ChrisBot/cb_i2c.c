#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#include "cb_i2c.h"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"


volatile status_t I2C_Status = DONE;
static volatile unsigned char data;
static volatile tBoolean receive;
static unsigned char address;

static status_t nocallback(unsigned char eh) {return DONE;}
static status_t (*callback)(unsigned char) = &nocallback;


void I2CIntHandler(void) {
	//while(I2CMasterBusy(I2C0_MASTER_BASE));
	I2CMasterIntClear(I2C0_MASTER_BASE);	
	
	if (receive) {
		data = I2CMasterDataGet(I2C0_MASTER_BASE);
		I2C_Status = DONE;
		I2C_Status = (*callback)(data);
	} else {
		I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, address>>1, receive=true);
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
	}
}

void I2C_Init() {
  // Init I2C Bus
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);	
	I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), true);
  I2CMasterEnable(I2C0_MASTER_BASE);

	I2CMasterIntEnable(I2C0_MASTER_BASE);
	IntEnable(INT_I2C0);
}

unsigned char I2C_Request(unsigned char a, unsigned char value) {
	I2C_Background_Request(a, value, 0);
	while(I2C_Status == BUSY);
	return data;
}

void I2C_Background_Request(unsigned char a, unsigned char value, status_t (*cb)(unsigned char)) {
	while(I2C_Status == BUSY);
	callback = cb ? cb : &nocallback;
	I2C_Status = BUSY;
	
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, (address = a)>>1, receive = false);
	I2CMasterDataPut(I2C0_MASTER_BASE, value);
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
}
