
#define LEFT_ENCODER 0
#define RIGHT_ENCODER 1

extern volatile signed long Encoder_Values[2];

void Encoder_Init(tBoolean, tBoolean);
signed long * Encoder_Read(void);
void Encoder_Background_Read(void (*)(signed long *));
void Encoder_Preset(signed long, signed long);
void Encoder_Callback(unsigned char, signed long, void (*)(void));
