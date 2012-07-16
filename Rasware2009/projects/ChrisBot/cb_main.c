#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "driverlib/gpio.h"

#include "RASLib/init.h"
#include "RASLib/timer.h"

#include "cb_adc.h"
#include "cb_compass.h"
#include "cb_sonar.h"
#include "cb_led.h"
#include "cb_encoder.h"
#include "cb_motor.h"
#include "cb_jumper.h"

#include "cb_travel.h"

#define IR_LEFT        6
#define IR_RIGHT       3
#define IR_FRONT_LEFT  5
#define IR_FRONT_RIGHT 4
#define IR_LONG_LEFT   2
#define IR_LONG_RIGHT  0
#define IR_FRONT       1

unsigned char c,d;
tBoolean usemotors;
unsigned char filtered_ir[7];
unsigned long filtered_sonar;
unsigned char panic_sensor;

void panic_ir(unsigned char);
void avoid_ir(unsigned char * adc) {
	signed char offset;
	for(c=0; c<7; c++)
		filtered_ir[c] = (adc[c]+filtered_ir[c]*9)/10;

	offset =
		(filtered_ir[IR_FRONT_LEFT]- filtered_ir[IR_FRONT_RIGHT])/100 + 
		(filtered_ir[IR_LONG_LEFT] - filtered_ir[IR_LONG_RIGHT] )/50;
	LED_Set(LED_0,offset+1);
	LED_Set(LED_1,1-offset);
	
	if (offset > 0)
		Motor_Set(127,127-offset);
	else
		Motor_Set(127+offset,127);
	
	if (filtered_ir[IR_FRONT] > 100 || Sonar_Value > 1750000) {
		if (usemotors) {
			if (filtered_ir[IR_FRONT_LEFT] > filtered_ir[IR_FRONT_RIGHT])
				Motor_Set(-127,127);
			else
				Motor_Set(127,-127);
		}
		ADC_Single_Background_Read(panic_sensor=IR_FRONT,&panic_ir);
	} else if (filtered_ir[IR_FRONT_LEFT] > 100) {
		if (usemotors) Motor_Set(-127,127);
		ADC_Single_Background_Read(panic_sensor=IR_FRONT_LEFT,&panic_ir);
	} else if (filtered_ir[IR_FRONT_RIGHT] > 100) {
		if (usemotors) Motor_Set(127,-127);
		ADC_Single_Background_Read(panic_sensor=IR_FRONT_RIGHT,&panic_ir);
	} else {
		ADC_Background_Read(&avoid_ir);
	}
	
	LED_Set(LED_3,d++);
	if (d==0) UARTprintf("off-->%d sonar-->%d d->%d\n",offset,filtered_sonar,d);
}

void avoid_sonar(unsigned long eh) {
	filtered_sonar = (eh + filtered_sonar*9)/10;
	Sonar_Background_Read(&avoid_sonar);
}
	
void panic_ir(unsigned char val) {
	
	filtered_ir[panic_sensor] = (val+filtered_ir[panic_sensor]*9)/10;
	
	if (filtered_ir[panic_sensor] < 100 && filtered_sonar < 1750000) {
		if (usemotors) Motor_Set(127,127);
		ADC_Background_Read(&avoid_ir);
	} else {
		ADC_Single_Background_Read(panic_sensor,&panic_ir);
	}
	
	LED_Set(LED_2,c++);
	if (c==0) UARTprintf("ir==>%d sonar==>%d sensor==>%d c=>%d\n",filtered_ir[panic_sensor],filtered_sonar,panic_sensor,c);
}



int main(void) {
	LockoutProtection();
	InitializeMCU();
	
	//init uart
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);				
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	
	UARTStdioInit(0);
	
	LED_Init();
	Jumper_Init();
	
	ADC_Init();
	Sonar_Init();
	
	usemotors = Jumper_Value & 0x8;
	
	if ((Jumper_Value & 0x7) == 0x1) {
		if (usemotors) {
			Motor_Init(false,true);
			Motor_Set(127,127);
		}
		avoid_sonar(0);
		avoid_ir(filtered_ir);
		for (;;);
	}
	
	if ((Jumper_Value & 0x7) == 0x2) {
		Travel_Init(usemotors);
		Travel_Go(FULL_SPEED);
		for (;;);
	}
	
	if ((Jumper_Value & 0x7) == 0x3) {
		if (usemotors) {
			Motor_Init(false,true);
			Motor_Set(127,127);
		}
		for (;;);
	}
	
	//if no jumpers are set, enter debug mode
	Encoder_Init(true,false);
	
	
	for (;;c++) {
		ADC_Background_Read(0);
		Sonar_Background_Read(0);
		Encoder_Background_Read(0);
		Jumper_Read();
		
		UARTprintf("ADC[%3d %3d %3d %3d %3d %3d %3d %3d]  S[%7d]  E[%3d %3d]  J[%1x] c:%d\n",
			ADC_Values[0],ADC_Values[1],ADC_Values[2],ADC_Values[3],ADC_Values[4],ADC_Values[5],ADC_Values[6],ADC_Values[7],
			Sonar_Value,
			Encoder_Values[0],Encoder_Values[1],
			Jumper_Value,
			c
		);
		
		LED_Set(LED_0,c);
		LED_Set(LED_1,c+64);
		LED_Set(LED_2,c+128);
		LED_Set(LED_3,c+192);

		WaitUS(20000);
	}
}

