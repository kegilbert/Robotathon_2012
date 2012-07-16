#define HMC6352 0x42

extern volatile unsigned char Compass_Values[2];

void Compass_Init(void);
unsigned char * Compass_Read(void);
void Compass_Background_Read(void (*)(unsigned char *));
