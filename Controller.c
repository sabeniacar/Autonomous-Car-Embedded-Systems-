#ifndef controlleyxD
#define controlleyxD

#include "LPC407x_8x_177x_8x.h"

#include "Library/GPIO.h"
#include "Library/SystemStructures.h"
#include "Library/SystemTickTimer.h"

uint32_t temp = 0;

void moveBackward(){
	
	PWM_Cycle_Rate(20);
	temp &= ~(1<<4);
	temp |= (1<<5);
	temp &= ~(1<<26);
	temp |= (1<<25);
	PORT0->PIN=temp;
	PWM_Wr1te(0,1);
	PWM_Wr1te(0,2);
	PWM_Wr1te(100,3);
	PWM_Wr1te(100,4);
	PWM_Wr1te(100,5);
	PWM_Wr1te(100,6);
}

void moveForward(){
	PWM_Cycle_Rate(20);
	temp |= (1<<4);
	temp &= ~(1<<5);
	temp |= (1<<26);
	temp &= ~(1<<25);
	PORT0->PIN=temp;
	PWM_Wr1te(100,1);
	PWM_Wr1te(100,2);
	PWM_Wr1te(0,3);
	PWM_Wr1te(0,4);
	PWM_Wr1te(100,5);
	PWM_Wr1te(100,6);
}

void turnLeft(){
	PWM_Wr1te(0,2);
	PWM_Wr1te(0,4);
	temp |= (1<<4);
	temp &= ~(1<<5);
	temp |= (1<<26);
	temp &= ~(1<<25);
	PORT0->PIN=temp;
	PWM_Wr1te(100,5);
	PWM_Wr1te(100,6);
	PWM_Cycle_Rate(125);
	PWM_Wr1te(50,1);
	PWM_Wr1te(50,3);
}
void turnRight(){
	PWM_Wr1te(0,1);
	PWM_Wr1te(0,3);
	temp |= (1<<4);
	temp &= ~(1<<5);
	temp |= (1<<25);
	temp &= ~(1<<26);
	PORT0->PIN=temp;
	PWM_Wr1te(100,5);
	PWM_Wr1te(100,6);
	PWM_Cycle_Rate(125);
	PWM_Wr1te(50,2);
	PWM_Wr1te(50,4);
}



void stop(){	
	PWM_Wr1te(0,1);
	PWM_Wr1te(0,2);
	PWM_Wr1te(0,3);
	PWM_Wr1te(0,4);
	PWM_Wr1te(0,5);
	PWM_Wr1te(0,6);

}
void powerDown(){
	SCR |= (1<<2);
	PCON |=0x3 ;
}

#endif
