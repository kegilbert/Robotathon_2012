
typedef enum {DONE,BUSY} status_t;
extern volatile status_t I2C_Status;

void I2C_Init(void);
void I2C_Get(unsigned char);
void I2C_Put(unsigned char, unsigned char);
unsigned char I2C_Request(unsigned char, unsigned char);
void I2C_Background_Request(unsigned char, unsigned char, unsigned char (*)(unsigned char));

