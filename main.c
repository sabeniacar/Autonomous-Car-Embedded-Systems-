#include "LPC407x_8x_177x_8x.h"


#include "Library/Joystick.h"
#include "Library/PWM.h"
#include "Library/PLL.h"
#include "Library/Ultrasonic.h"
#include "Library/SystemStructures.h"
#include "Library/SystemTickTimer.h"
#include "Library/ADC.h"
#include "Library/Controller.h"

#include "Library/LED.h"
#include "Library/Serial.h"
#include "Library/ESP8266.h"
#include "Library/WIFI.h"
#include "Library/I2C.h"



char* atHttpGetCmd ;
int length ;
char str[100];
int i=0;
uint32_t out;
void init() {	
	SysTick_Init();
	//PLL_Change_Configuration();
	Ultrasonic_Trigger_Timer_Init();
	Ultrasonic_Capture_Timer_Init();
	Ulrasonic_Start_Trigger();
	//Joystick_Init();
	ADC_Init();
	ADC_Start();
		
	out = PORT0->DIR;	//GPIO pins to be used for motor controller, set their directions
	//4,5 left motor GPIO pins
	out |= (1<<4);
	out |= (1<<5);
	//6,7 right motor GPIO pins
	out |= (1<<8);
	out |= (1<<7);
	PORT0->DIR=out;
	temp = PORT0->PIN;

	PWM_Init();	
	LED1_Init();
	LED2_Init();
	LED3_Init();
	LED4_Init();
	
	LED1_Off();
	LED2_Off();
	LED3_Off();
	LED4_Off();
		
   Serial_Init();	
	//WIFI_Init();
	I2C_Init();

}

void SysTick_Handler() {
	ADC_Start();
}

// When code start, LED should be turned off.
// When you press the UP button of the Joystick, LED will be turned on (use 100% Duty Cycle)
// When you press the CENTER button of the Joystick, LED will be turned on (use 50% Duty Cycle)
// When you press the DOWN button of the Joystick, LED will be turned off.
// When you press the LEFT button of the Joystick, LED will blink (2 times in a second).
// When you press the RIGHT button of the Joystick, LED will blink (10 times in a second).

//Determine  the action upon joystick press



//Checks if there is any input from the joystick. Calls helper functions accordingly. 
//Returns 0 if there's no input from the joystick, 1 otherwise.

int joystick_handler(){
	
	if(Joystick_Up_Pressed()){
		moveForward();
		return 1;
	}else if(Joystick_Left_Pressed()){
		turnLeft();
		return 1;
	}else if(Joystick_Down_Pressed()){
		moveBackward();
		return 1;
	}else if(Joystick_Right_Pressed()){
		turnRight();
		return 1;
	}else if(Joystick_Center_Pressed()){
		powerDown();
		__WFI();
		return 1;
	}
	return 0;
}

uint32_t joystick=0;
//Checks if the ultrasonic sensor detected an obstacle that is closer than 10 centimeters. 
//Calls helper functions accordingly.
int sensor_handler(){
	if(ultrasonicSensorEdgeCount<2)
		return 0;
	if(distance==0)
		return 0;
	if(distance<10){
			stop();
			while(distance<10){
				if(joystick_handler()){
					return 0;
				}
			}
				moveForward();
			  return 0;
		}
	return 0;
}


uint32_t left=0;
uint32_t right=0;

//Reads values from ADC converters if they are ready. Returns 2 if there's significantly more light on the left side,
//1 if there's significantly more light on the right side, 0 otherwise.
int LDRState(){
		//if data is not available on both LDRs return
		if(ADC_New_Data_Available_Right!=1 | ADC_New_Data_Available_Left!=1){
			return -1;
		}
		left =  ADC_GetLastValueLeft();
		right = ADC_GetLastValueRight();
		if(left>right &&  left-right>300){
				return 1;
		}else if(left<right && right-left>300) {
				return 2;
		}
		return 0;
	}

volatile uint32_t ldrState = 0;
volatile uint32_t turn = 0;
// Checks LDR measurements. Manipulates the car's direction accordingly.
	int ldr_handler(){
			ldrState = LDRState();
			if(ldrState==2){
				turnLeft();
				turn=1;
			}else if(ldrState==1){
				turnRight();
				turn=1;
			}
			else if(ldrState==0 && turn==1){
					turn=0;
					moveForward();
			}
			
			
			return 0;
	}

int select_scenario(){
	if(Joystick_Left_Pressed()){
		return 1;
	}else if(Joystick_Down_Pressed()){
		return 3;
	}else if(Joystick_Right_Pressed()){
		return 2;
	}else if(Joystick_Center_Pressed()){
		powerDown();
		__WFI();
		return 5;
	}
	return 5;
}

void followRoad(){
  if(sensor_handler()){}
	else if(ldr_handler()){}
}
void followWIFI(){
  if(sensor_handler()){}
	else if(ldr_handler()){}

}

//Checks each handler function in priority order. E.g. When there is joystick input \textit{and} an object within 
//10 centimeters, program flows through joystick input and ignores the object because of the way the if-else structure 
//is formed.
uint32_t speed=0;
uint32_t scenario=0;
uint32_t tempscenario=0;
uint32_t forward=0;

char* wakeUp="k\0";

void update() {		
//	I2C_Write(68, "C", 1, 1);
	
	
	//read register 6B   0x40
	//if(!I2C_Write(68, sleep, 2, 1)){
			/*if(!I2C_Write(68, data, 1, 0)){
				if(!I2C_Read(68, data2, 1, 1)){
							serialTransmitData="jj";
							Serial_WriteData(*serialTransmitData++);
							while(!serialTransmitCompleted);
				}
			}
			*/
	//}
	

	tempscenario = select_scenario();
	if(tempscenario!=5){
		scenario=tempscenario;
	}
	if(scenario==1){
		if(forward==0){
			moveForward();
			forward++;
		}
		followRoad();
	}else if(scenario==2){
		followWIFI();
	}else if(scenario==3){
	}

	//Calculate speed from trimpot value. 	
	speed = getSpeed();
	
	// Adjust speed based on trimpot value.
	if(dir == MOTOR_LEFT){
		PWM1_Wr1te(speed,2);
	}else if (dir == MOTOR_RIGHT){
		PWM1_Wr1te(speed,1);
	}else if(dir==MOTOR_FORWARD){
		PWM1_Wr1te(speed,1);
		PWM1_Wr1te(speed,2);
	}else if(dir==MOTOR_BACKWARD){
		PWM1_Wr1te(speed,1);
		PWM1_Wr1te(speed,2);
	}

	__WFI();

}

int main() {
	init();
	
if(!I2C_Write(68, wakeUp, 2, 1)) //wakeup mpu6050
	
	while(1) {
		update();
	}
}


