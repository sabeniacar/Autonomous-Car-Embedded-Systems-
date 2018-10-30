#include "PWM.h"

void PWM_Init() {
	
	IOCON_FRONT_LEFT_LED |= 0x3;
	IOCON_FRONT_RIGHT_LED |= 0x3;
	IOCON_BACK_LEFT_LED |= 0x3;
	IOCON_BACK_RIGHT_LED |= 0x3;
	IOCON_MOTORA |= 0x2; 
	IOCON_MOTORB |= 0x2; 
	
	//Turn on PWM
	PCONP |=(1<<5);
	PCONP |=(1<<6);
	
	//Enable PWM output for corresponding pin.
	PWM0->PCR |= (15<<9);
	PWM1->PCR |= (3<<12);
	
	//Reset The PWM Timer Counter and The PWM Prescale Counter on the Next Positive Edge of PCLK
	PWM0->TCR |=(1<<1);
	PWM1->TCR |=(1<<1);
	
	//Configure MR0 register for giving pulse every 20 ms.
	PWM0->MR0 = 1200000;
	PWM1->MR0 = 1200000;

	
	//Reset TC, when MR0 matches TC.
	PWM0->MCR |=(1<<1);
	PWM1->MCR |=(1<<1);
	//Enable PWM Match 0 Latch.
	PWM0->LER |=(1<<0);
	PWM1->LER |= (1<<0);
	//Enable Counter and PWM and Clear Reset on the PWM
	PWM0->TCR |=(1<<0) | (1<<3);
	PWM0->TCR &=~(1<<1);
	PWM1->TCR |=(1<<0) | (1<<3);
	PWM1->TCR &=~(1<<1);	
	PWM_Wr1te(0,1);
	PWM_Wr1te(0,2);
	PWM_Wr1te(0,3);
	PWM_Wr1te(0,4);
	PWM1_Wr1te(0,1);
	PWM1_Wr1te(0,2);
	PWM1_Cycle_Rate(5);

}

void PWM_Cycle_Rate(uint32_t period_In_Cycles) {
//Write a formula that changes the MR0 register value for a given parameter.
	PWM0->MR0 = 60*1000*period_In_Cycles;
	//Enable PWM Match Register Latch for MR0.
	PWM0->LER |=(1<<0);
}

void PWM_Write(uint32_t T_ON) {	
	if(T_ON > 100) {
		T_ON = 100;
	}
	
	//Write a formula to calculate the match register for P30 (P1_2) pin.
	
	//If T_ON equal to MR0, there will be 1 Cycle Dropout. In order to prevent this, we increase the T_ON value.
	if (T_ON == PWMX->MR0) {
		T_ON++;
	}
	PWMX->MR1 = T_ON;
	
	//Enable PWM Match Register Latch.
}
volatile uint32_t *mr_address;
//Modified version of PWM_Write in base code
//dir = 1,2,3,4 for leds
void PWM_Wr1te(uint32_t T_ON,const uint32_t dir) {	
	if(T_ON > 100) {
		T_ON = 100;
	}
	/* */if(dir==1) 	mr_address=&(PWM0->MR1);
	else if(dir==2) 	mr_address=&(PWM0->MR2);
	else if(dir==3) 	mr_address=&(PWM0->MR3);
	else if(dir==4) 	mr_address=&(PWM0->MR4);

	//Write a formula to calculate the match register for P30 (P1_2) pin.
	*mr_address = (PWM0->MR0*T_ON/100);
	//If T_ON equal to MR0, there will be 1 Cycle Dropout. In order to prevent this, we increase the T_ON value.
	if (*mr_address == PWM0->MR0) {
		(*mr_address)++;
	}
	//Enable PWM Match Register Latch.
	PWM0->LER |=(1<<dir);
}

void PWM1_Cycle_Rate(uint32_t period_In_Cycles) {
//Write a formula that changes the MR0 register value for a given parameter.
	PWM1->MR0 = 60*1000*period_In_Cycles;
	//Enable PWM Match Register Latch for MR0.
	PWM1->LER |=(1<<0);
}

//Modified version of PWM_Write in base code

void PWM1_Wr1te(uint32_t T_ON,const uint32_t dir) {	
	if(T_ON > 100) {
		T_ON = 100;
	}

	/* */if(dir==1) 	mr_address=&(PWM1->MR4);
	else if(dir==2) 	mr_address=&(PWM1->MR5);
	//Write a formula to calculate the match register for P30 (P1_2) pin.
	*mr_address = (PWM1->MR0*T_ON/100);
	//If T_ON equal to MR0, there will be 1 Cycle Dropout. In order to prevent this, we increase the T_ON value.
	if (*mr_address == PWM1->MR0) {
		(*mr_address)++;
	}
	//Enable PWM Match Register Latch.
	PWM1->LER |=(1<<(dir+3));
}

