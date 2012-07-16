#define ADS7830 0x90

//Array that is filled
extern volatile unsigned char ADC_Values[8];

//Functions used to filled Array of values
void ADC_Init(void);
unsigned char ADC_Single_Read(unsigned char);
void ADC_Single_Background_Read(unsigned char, void (*)(unsigned char));
unsigned char * ADC_Read(void);
void ADC_Background_Read(void (*)(unsigned char *));
