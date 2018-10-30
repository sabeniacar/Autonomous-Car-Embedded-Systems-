#ifndef I2C_H
#define I2C_H

#include "LPC407x_8x_177x_8x.h"

#include "SystemStructures.h"

typedef struct
{
  volatile uint32_t CONSET;
  volatile uint32_t STAT;
  volatile uint32_t DAT;
  volatile uint32_t ADR0;
  volatile uint32_t SCLH;
  volatile uint32_t SCLL;
  volatile uint32_t CONCLR;
  volatile uint32_t MMCTRL;
  volatile uint32_t ADR1;
  volatile uint32_t ADR2;
  volatile uint32_t ADR3;
  volatile uint32_t DATA_BUFFER;
  volatile uint32_t MASK0;
  volatile uint32_t MASK1;
  volatile uint32_t MASK2;
  volatile uint32_t MASK3;
} I2C_TypeDef;

//Write the base address of the I2C.
#define I2C_BASE	0x4001C000
#define I2C	((I2C_TypeDef*) I2C_BASE)

//Write the IOCON address of I2C Data Input/Output.
#define I2C_SDA_PIN_ADDRESS 0x4002C288
#define I2C_SDA_PIN	*((volatile uint32_t*)(I2C_SDA_PIN_ADDRESS))

//Write the IOCON address of I2C Clock Input/Output. 
#define I2C_SCL_PIN_ADDRESS 0x4002C28C
#define I2C_SCL_PIN	*((volatile uint32_t*)(I2C_SCL_PIN_ADDRESS))

//Write I2CDutyCycle which is (I2CSCLL + I2CSCLH) / 2
#define I2CDutyCycle 300	

void I2C_Init(void);
int I2C_Start(void);
int I2C_Wait_SI(void);

int I2C_Write(uint32_t address, char* data, int length, int stop);
int I2C_WriteData(uint32_t address,const char* data,int length, int stop);
int I2C_Do_Write(int value);

int I2C_Read(uint32_t address, char* data, int length, int stop);
int I2C_ReadData(uint32_t address,char* data,int length, int stop);
int I2C_Do_Read(int isLast);

#endif
