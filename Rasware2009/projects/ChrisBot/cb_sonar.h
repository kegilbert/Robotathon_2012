
extern volatile unsigned long Sonar_Value;

void Sonar_Init(void);
unsigned long Sonar_Read(void);
void Sonar_Background_Read(void (*)(unsigned long));
