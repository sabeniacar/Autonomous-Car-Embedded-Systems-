#include "Serial.h"

char* serialTransmitData = 0;
uint8_t serialTransmitCompleted = 0;
char serialReceivedCharacter = 0;

void Serial_Init() {
	Serial_UART_TX_PIN |= 0x01;
	Serial_UART_RX_PIN |= 0x01;
	
	PCONP |= 1 <<  3;
	
	Serial_UART->FCR =	1 << 0
						 |	0 << 1
						 |	0 << 2
						 |	0 << 6;
	
	Serial_UART->LCR |= (1 << 7);
	
	Serial_UART->DLM = 0x01;
	Serial_UART->DLL = 0x25;
	Serial_UART->FDR = 0x01 << 0 | 0x03 << 4;

	Serial_UART->LCR &= ~(1 << 7);
	
	Serial_UART->LCR =	3 << 0
							| 0 << 2
							| 1 << 3
							| 1 << 4;
							
	Serial_UART->IER |= 1 << 0 | 1 << 1;
	
	NVIC_EnableIRQ(UART0_IRQn);
	
	NVIC_SetPriority(UART0_IRQn,5);
}

void UART0_IRQHandler() {	
	uint32_t currentInterrupt = ((Serial_UART->IIR & (0x7 << 1)) >> 1);
	
	if(currentInterrupt == 0x02) {
		serialReceivedCharacter = Serial_ReadData();
	}
	else if(currentInterrupt == 0x01) {
		if(*serialTransmitData > 0) {
			Serial_WriteData(*serialTransmitData++);
		}
		else {
			serialTransmitCompleted = 1;
		}
	}
}

char Serial_ReadData() {
	return Serial_UART->RBR;
}

void Serial_WriteData(char data) {
	serialTransmitCompleted = 0;
	Serial_UART->THR = data;
}

