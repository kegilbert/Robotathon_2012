#define J_PIN_0 ((Jumper_Value&0x1)>>0)
#define J_PIN_1 ((Jumper_Value&0x2)>>1)
#define J_PIN_2 ((Jumper_Value&0x4)>>2)
#define J_PIN_3 ((Jumper_Value&0x8)>>3)

extern unsigned char Jumper_Value;

void Jumper_Init(void);
unsigned char Jumper_Read(void);
