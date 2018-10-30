#include "LPC407x_8x_177x_8x.h"

#include "GPIO.h"
#include "SystemStructures.h"
#include "SystemTickTimer.h"
#include "PWM.h"
#include "Controller.h"
#include "ADC.h"
#include "I2C.h"

volatile uint32_t temp = 0;
uint32_t dir = 0;

volatile uint32_t tr=0;

// P0[4] and P0[5] pins are left motor's in1 and in2
// if P0[4] is 0 and P0[5] is 1:   left-motor goes forward
// if P0[4] is 1 and P0[5] is 0:   left-motor goes backward
// motor doesn't move otherwise.
void left_motor_direction(uint32_t _direction){
	temp = PORT0->PIN;
	if(_direction == MOTOR_FORWARD){
		temp |= (1<<5);
		temp &= ~(1<<4);
	}
	else if(_direction == MOTOR_BACKWARD){
		temp &= ~(1<<5);
		temp |= (1<<4);
	}
	PORT0->PIN = temp;
}


// P0[6] and P0[7] pins are right motor's in1 and in2
// if P0[6] is 0 and P0[7] is 1:   left-motor goes forward
// if P0[6] is 1 and P0[7] is 0:   left-motor goes backward
// motor doesn't move otherwise.
void right_motor_direction(uint32_t _direction){
	temp = PORT0->PIN;
	if(_direction == MOTOR_FORWARD){
		temp &= ~(1<<7);
		temp |= (1<<8);
	}
	else if(_direction == MOTOR_BACKWARD){
		temp |= (1<<7);
		temp &= ~(1<<8);
	}
	PORT0->PIN = temp;
}


// reads trimpot value into tr.
int getSpeed(){
		tr = ADC_GetLastValueTrimpot();
		return 50.0*tr/0xFFF+50;
}

// point turn functions <<------------------------- YENI YAZDIK ---------------------------->>

void pointLeft(void){
	left_motor_direction(MOTOR_BACKWARD);
	right_motor_direction(MOTOR_FORWARD);
	PWM1_Wr1te(20,1); 
	PWM1_Wr1te(20,2);
}

void pointRight(void){
	left_motor_direction(MOTOR_FORWARD);
	right_motor_direction(MOTOR_BACKWARD);
	PWM1_Wr1te(20,1); 
	PWM1_Wr1te(20,2);
}


char* gyroAddr="G"; //address of GYRO_ZOUT_H 
char gyroVals[7] = {};

void rotate_left_90(void){
	uint32_t angle = 0;
	char gyro_val[2];

	// start turning left
	pointLeft();
	while(angle < 87){
		if(!I2C_Write(68, gyroAddr, 1, 0)){			//register to be read is the start address of gyros
			if(!I2C_Read(68, gyroVals, 2, 1)){   //read gyro registers
				// we have successfully read gyro data
				uint32_t reading = (gyroVals[1]<<8) + gyroVals[0];
				reading &= ~(1<<16); // get rid of the sign bit
				angle += reading*0.001/131.0;
			}
		}		
	}
	
}

void moveBackward(){
	dir= MOTOR_BACKWARD;
	PWM_Cycle_Rate(20);
	left_motor_direction(MOTOR_BACKWARD);
	right_motor_direction(MOTOR_BACKWARD);
	/*temp &= ~(1<<5);
	temp |= (1<<4);
	temp &= ~(1<<6);
	temp |= (1<<7);
	PORT0->PIN=temp;
	*/
	PWM_Wr1te(0,1);   // turn off front-left led
	PWM_Wr1te(0,2);   // turn off front-right led
	PWM_Wr1te(100,3); // turn on back-left led
	PWM_Wr1te(100,4); // turn on back-right led
	PWM1_Wr1te(getSpeed(),1); // maps tr(who is in range 0 and 0xFFF) to 0,100 range 
	PWM1_Wr1te(getSpeed(),2);
}

void moveForward(){
	dir = MOTOR_FORWARD;
	PWM_Cycle_Rate(20);
	/*temp |= (1<<5);
	temp &= ~(1<<4);
	temp |= (1<<6);
	temp &= ~(1<<7);
	PORT0->PIN=temp;*/
	left_motor_direction(MOTOR_FORWARD);
	right_motor_direction(MOTOR_FORWARD);
	
	PWM_Wr1te(100,1); // turn on front-left led
	PWM_Wr1te(100,2); // turn on front-right led
	PWM_Wr1te(0,3);   // turn off back-left led
	PWM_Wr1te(0,4);   // turn off back-right led
	PWM1_Wr1te(getSpeed(),1); // maps tr(who is in range 0 and 0xFFF) to 0,100 range 
	PWM1_Wr1te(getSpeed(),2);
}

void turnLeft(){
	dir = MOTOR_LEFT;
	getSpeed();       // read trimpot value
	PWM_Wr1te(0,2);   // turn off front-right led
	PWM_Wr1te(0,4);   // turn off back-right led
	
	right_motor_direction(MOTOR_FORWARD);
	left_motor_direction(MOTOR_BACKWARD);
	PWM1_Wr1te(40,1); // stop the left motor
	PWM1_Wr1te(getSpeed(),2); // maps tr(who is in range 0 and 0xFFF) to 0,100 range 
	PWM_Cycle_Rate(125);
	PWM_Wr1te(50,1);   // make front-left led blink
	PWM_Wr1te(50,3);   // make back-left  led blink 
}
void turnRight(){
	dir = MOTOR_RIGHT;
	getSpeed();       // read trimpot value
	PWM_Wr1te(0,1);   // turn off front-left led
	PWM_Wr1te(0,3);   // turn off back-left led
	/*temp |= (1<<5);
	temp &= ~(1<<4);
	temp |= (1<<7);
	temp &= ~(1<<6);
	PORT0->PIN=temp;*/
	left_motor_direction(MOTOR_FORWARD);
	right_motor_direction(MOTOR_BACKWARD);

	PWM1_Wr1te(getSpeed(),1); // maps tr(who is in range 0 and 0xFFF) to 0,100 range 
	PWM1_Wr1te(40,2); // stop the right motor
	PWM_Cycle_Rate(125);
	PWM_Wr1te(50,2);   // make front-right led blink
	PWM_Wr1te(50,4);   // make back-right  led blink
}


// turns off all the leds and motors.
void stop(){	
	dir = 0;
	PWM_Wr1te(0,1);
	PWM_Wr1te(0,2);
	PWM_Wr1te(0,3);
	PWM_Wr1te(0,4);
	PWM1_Wr1te(0,1);
	PWM1_Wr1te(0,2);
}

// sets up the powerdown conditins
// after powerDown() is executed, 
// system goes to powerdown when __WFI() is seen.
void powerDown(){
	SCR |= (1<<2);
	PCON |=0x3 ;
}
