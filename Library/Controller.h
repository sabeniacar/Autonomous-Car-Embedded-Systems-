#ifndef qontroller_he
#define qontroller_he

extern volatile uint32_t temp;

void moveBackward(void);
void moveForward(void);
void turnLeft(void);
void turnRight(void);
void stop(void);
void powerDown(void);

// point turn functions <<----- YENI YAZDIK
void pointLeft(void);
void pointRight(void);

void right_motor_direction(uint32_t);
void left_motor_direction(uint32_t);
int getSpeed(void);

extern uint32_t dir;
extern char gyroVals[7];
extern char* gyroAddr;


#define MOTOR_FORWARD    1234
#define MOTOR_BACKWARD   4321
#define MOTOR_LEFT    1324
#define MOTOR_RIGHT   1243

#endif
