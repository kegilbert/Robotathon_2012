
// Definitions of corners
#define FLAG 0
#define FAN 1 //couldn't remember name of this one
#define ELECTRIC 2
#define LIGHT 3
#define TREE -1

 
#define offset(x) (currentFacing?((x)+3)%6:(x))
#define tripped(x,y) (ADS7830_Values[offset(x)]>bounds[y][offset(x)])
#define till(t) for(;!(t);SetMotorPowers(motor_L,motor_R))
#define diff(x,y) (bounds[y][offset(x)]-ADS7830_Values[offset(x)])

void travelInit(void);

//calculates PD based on constants given
signed char PID(unsigned char,unsigned char,signed short,signed short);

//corner to goto, and boolean value for turning backwards to use fan
void stop(void);
void goForward(void);
void goForwardBlocking(void);
void goBackward(void);
void goEngageCorner(signed char);
void goWall(void);
void goRotate(signed char);
void goAlignWall(char,char);
void gotoDest(signed char);
void WallFollow(int mode, int time, int dir);
void BackOut(void);
void testSensors(void);

 
