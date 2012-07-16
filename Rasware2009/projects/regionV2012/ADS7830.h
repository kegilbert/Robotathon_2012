#define ADS7830 (0x90)

extern unsigned char ADS7830_Values[8];

unsigned char I2CGet(void);
void I2CRequest(unsigned char);

void ADS7830_Init(void);
void ADS7830_Read(void);
void ADS7830ReadNonBlocking(void);
void I2CIntHandler(void);
