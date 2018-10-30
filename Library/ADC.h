#ifndef ADC_H
#define ADC_H

#include "LPC407x_8x_177x_8x.h"

#include "SystemStructures.h"

typedef struct {
  volatile	uint32_t CR;
  volatile	uint32_t GDR;
						uint32_t RESERVED0;
  volatile	uint32_t INTEN;
  volatile	uint32_t DR[8];
  volatile	uint32_t STAT;
  volatile	uint32_t TRM;
} ADC_TypeDef;

#define ADC_CLOCK_FREQUENCY 1000000

//Write the IOCON address of Analog Pin
#define	ANALOG_PIN_LEFT_IOCON_ADDRESS	0x4002C0F8
#define ANALOG_PIN_LEFT_IOCON	*((volatile uint32_t*)(ANALOG_PIN_LEFT_IOCON_ADDRESS))
	
//Write the IOCON address of Analog Pin
#define	ANALOG_PIN_RIGHT_IOCON_ADDRESS	0x4002C0FC
#define ANALOG_PIN_RIGHT_IOCON	*((volatile uint32_t*)(ANALOG_PIN_RIGHT_IOCON_ADDRESS))

//Write the IOCON address of Analog Pin
#define	ANALOG_PIN_TRIMPOT_IOCON_ADDRESS	0x4002C05C
#define ANALOG_PIN_TRIMPOT_IOCON	*((volatile uint32_t*)(ANALOG_PIN_TRIMPOT_IOCON_ADDRESS))
	

//Write the CLKDIV of ADC for given ADC_CLOCK_FREQUENCY.
#define ADC_CLKDIV 0x15 
//Write the max value of ADC.
#define ADC_MAX_VALUE 0xFFF

#define ADC_BASE	0x40034000
#define ADC	((ADC_TypeDef*) ADC_BASE)

extern volatile  uint32_t ADC_Last_Left;
extern volatile uint32_t ADC_Last_Right;
extern volatile uint32_t ADC_Last_Trimpot;

extern uint8_t ADC_New_Data_Available_Left;
extern uint8_t ADC_New_Data_Available_Right;
extern uint8_t ADC_New_Data_Available_Trimpot;

//asagi yeni
extern uint32_t hold_direction;
//extern uint32_t temp;
extern uint32_t turning;
extern volatile uint32_t lastvalue;
extern volatile uint32_t drvalue;
extern volatile uint8_t channel;

void ADC_Init(void);
void ADC_Start(void);
void ADC_Stop(void);
uint32_t ADC_GetLastValueLeft(void);
uint32_t ADC_GetLastValueRight(void);
uint32_t ADC_GetLastValueTrimpot(void);



#endif
