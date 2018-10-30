#include "PLL.h"

void PLL_Init() {
	CCLKSEL = 0x01;
	PCLKSEL = 0x04;
	EMCCLKSEL = 0x00;
	USBCLKSEL = 0x00;
	SPIFICLKSEL = 0x00;
}

void PLL_Disable() {
	//Disable PLL
	 PLLCON &= ~(1<<0);
}

void PLL_Feed() {
	//Feed PLL
	PLLFEED = 0xAA;
	PLLFEED = 0X55;
}

void PLL_Change_Source() {
	//Select the clock source as the OSC_CLK
	CLKSRCSEL |= (1<<0);
}

void PLL_Change_Frequency() {
	//Change PLL Frequency to 60MHz
	PLLCFG = 0x24;
}

void PLL_Enable() {
	//Enable PLL
	PLLCON |= (1<<0);
}

void PLL_Check_State() {
	//Check PLL is Enabled
	while((PLLSTAT & (1<<10))!=(1<<10));
}

void PLL_Start() {
	//Select the CPU clock as the same clock rate as Main PLL
	CCLKSEL = (1<<8 | 1<<0);
	//Configure PCLK as PCLKDIV = 0x04
	PCLKSEL|=0x04;
	
	EMCCLKSEL = 0x01;
	USBCLKSEL = 0x201;
	SPIFICLKSEL = 0x02;
}

void PLL_Change_Configuration() {
	PLL_Init();
	PLL_Disable();
	PLL_Feed();
	PLL_Change_Source();
	PLL_Change_Frequency();
	PLL_Enable();
	PLL_Feed();
	PLL_Check_State();
	PLL_Start();
}

