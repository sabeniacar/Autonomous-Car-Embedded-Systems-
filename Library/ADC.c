#include "ADC.h"
#include "PWM.h"
#include "GPIO.h"
#include "Controller.h"

volatile uint32_t stopp = 0;
volatile uint32_t start = 0;
volatile uint32_t tt = 0;
volatile uint32_t ADC_Last_Left = 0;
volatile uint32_t ADC_Last_Right = 0;
volatile uint32_t ADC_Last_Trimpot = 0;
uint8_t ADC_New_Data_Available_Left = 0;
uint8_t ADC_New_Data_Available_Right = 0;
uint8_t ADC_New_Data_Available_Trimpot = 0;



void ADC_Init() {
	//Change the function value of pin to ADC.
	ANALOG_PIN_LEFT_IOCON	 |= (1<<0);
	ANALOG_PIN_RIGHT_IOCON	 |= (1<<0);
	ANALOG_PIN_TRIMPOT_IOCON	 |= (1<<0);


	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	ANALOG_PIN_LEFT_IOCON &= ~(3<<3);
	ANALOG_PIN_RIGHT_IOCON	 &= ~(3<<3);
	ANALOG_PIN_TRIMPOT_IOCON	 &= ~(3<<3);


	//Change Analog/Digital mode of pin to Analog.
	ANALOG_PIN_LEFT_IOCON &= ~(1<<7);
	ANALOG_PIN_RIGHT_IOCON	 &= ~(1<<7);
	ANALOG_PIN_TRIMPOT_IOCON	 &= ~(1<<7);

	
	//Turn on ADC.
	PCONP |= (1<<12);
	
	//Select corresponding AD pins to be sampled and converted. Also, set the CLKDIV and make the A/D converter operational without Burst mode.
	tt = ADC->CR;
	tt &= ~(255);
	/*
	ADC->CR &= ~(1<<1);
	*/
	tt |= 0x31;
	
	tt |= (1<<21);
	tt |=(1<<16);
	tt |= (ADC_CLKDIV<<8);
	
	ADC->CR = tt;
	
	//Enable interrupt for corresponding pin.
//only global interrupt is enabled
		
		ADC->INTEN |= (1<<8);



	//Enable ADC_IRQn (Interrupt Request).
	NVIC_EnableIRQ(ADC_IRQn);
	
}
//Stops ADC by turning off burst mode.
//Note: ADC automatically stops when the burst mode is turned off since ADC's start bit is 0.
void ADC_Stop () {
	//burst mode stop
	stopp = ADC->CR;
	stopp &=~(1<<16);
	ADC->CR = stopp;
}
//adc start Starts ADC. By turning on burst mode.
void ADC_Start() {
	//burst mode start
	start =  ADC->CR;
	start |=(1<<16);
	ADC->CR = start;
}

uint32_t ADC_GetLastValueLeft() {
	ADC_New_Data_Available_Left = 0;
	return ADC_Last_Left;
}
uint32_t ADC_GetLastValueRight() {
	ADC_New_Data_Available_Right = 0;
	return ADC_Last_Right;
}
uint32_t ADC_GetLastValueTrimpot() {
	ADC_New_Data_Available_Trimpot = 0;
	return ADC_Last_Trimpot;
}


volatile uint32_t global = 0 ;
volatile uint8_t channel = 0 ;
volatile uint32_t result = 0 ;
volatile uint32_t done = 0 ;
volatile uint8_t count4 = 0;
volatile uint8_t count5 = 0;
volatile uint32_t sum4 = 0;
volatile uint32_t sum5 = 0;


//Reads GDR's(global data register) value whenever an interrupt is thrown. Reads and stores the AD converted values of
//Trimpot, Left LDR and Right LDR in their respective registers. Takes 10 samples and averages LDR readings before 
//storing them. After channel 5 is read, ADC stops converting until systickTimer starts it again.
void ADC_IRQHandler() {
		//ADC_Stop();
	  //Global ADC Data Register
		global = ADC->GDR;
		//ADC Channel
		channel = (global>>24) & 0x7 ;
		//ADC Result
		result = (global>>4) & 0xFFF;
		//Done Bit
		done = (global>>31) & 0x1;
	
		// Update Values
		if(done){
		
			if(channel == 4){
				
				sum4 += result;
				count4++;
				if(count4 == 10){
						ADC_New_Data_Available_Left = 1;
					  ADC_Last_Left = sum4/10.0; 
						count4=0;
						sum4 = 0;
				}
				
			}else if(channel == 5){
				
				sum5 += result;
				count5++;
				if(count5 == 10){
						ADC_New_Data_Available_Right = 1;
					  ADC_Last_Right = sum5/10.0; 
						count5=0;
						sum5 = 0;
					
				}
				
				ADC_Stop();
				
				
			}else{
			
				ADC_Last_Trimpot=result;
				ADC_New_Data_Available_Trimpot = 1;
			
			}
		
		}

}
