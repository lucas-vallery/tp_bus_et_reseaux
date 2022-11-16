/*
 * shell.c
 *
 *  Created on: Oct 22, 2022
 *      Author: lucas
 */
#include "shell.h"

extern uint8_t lastCharReceived;
extern Shell shell;
extern stepper_t stepper;

const uint8_t shellReturn[2] = {'\r', '\n'};
char charBuffer[BUFFER_SIZE] = {'\0'};
int indexBuffer = 0;

const char *tabCmd[]= {
		"GET_T",
		"GET_P",
		"SET_K",
		"GET_K",
		"GET_A",
		0
};


void shell_init(Shell* shell, UART_HandleTypeDef* uartHandler, USART_TypeDef* uartRegisters){
	shell->uartHandler = uartHandler;
	shell->uartRegisters = uartRegisters;
}

void shell_startRxIt(Shell* shell){
	HAL_UART_Receive_IT(shell->uartHandler,(uint8_t*) &lastCharReceived, 1);
}

void shell_clearBuffer(char buffer[], int bufferSize){
	int i;
	for(i = 0; i < bufferSize; i++) {
		buffer[i] = '\0';
	}
}

void shell_charReceived(Shell* shell){

	if(lastCharReceived == '\r' || indexBuffer >= BUFFER_SIZE){
		HAL_UART_Transmit(shell->uartHandler, (uint8_t*) &shellReturn, 2, 0x1F4);

		shell_executeCmd(charBuffer);
		shell_clearBuffer(charBuffer, BUFFER_SIZE);
		indexBuffer = 0;
	} else{
		HAL_UART_Transmit(shell->uartHandler, (uint8_t*) &lastCharReceived, 1, 0x1F4);
		charBuffer[indexBuffer] = lastCharReceived;
		indexBuffer++;
	}
}

void shell_executeCmd(){
	int selectedCmd = -1;
	char dataToSend[32] = {'\0'};

	for(int i = 0; tabCmd[i]; i++) {
		if(!strncmp(charBuffer, tabCmd[i], 5)){
			selectedCmd = i;
		}
	}

	switch(selectedCmd){
	case 0 :
		printf("GET_T command has been used!\r\n");
		float temp = BMP280_readCompensateTemp();
		sprintf(dataToSend, "%.1f\r", temp);
		HAL_UART_Transmit(shell.uartHandler, (uint8_t*)&dataToSend, 4, HAL_MAX_DELAY);
		break;
	case 1 :
		printf("GET_P command has been used!\r\n");
		float press = BMP280_readCompensatePress()/100.0;
		sprintf(dataToSend, "%.1f\r", press);
		HAL_UART_Transmit(shell.uartHandler, (uint8_t*)&dataToSend, 4, HAL_MAX_DELAY);
		break;
	case 2 :
		printf("SET_K command has been used!\r\n");
		/*
		char buff[4];
		snprintf(buff, 4, charBuffer);
		uint16_t k =  atoi(buff);
		stepper_setK(&stepper, k);
		*/
		break;
	case 3 :
		printf("GET_K command has been used!\r\n");
		uint16_t k = stepper_getK(&stepper);
		sprintf(dataToSend, "%d", k);
		HAL_UART_Transmit(shell.uartHandler, (uint8_t*)&dataToSend, 4, HAL_MAX_DELAY);
		break;
	case 4 :
		printf("GET_A command has been used!\r\n");
		uint16_t angle = stepper_getK(&stepper)*((uint16_t)BMP280_readCompensateTemp());
		sprintf(dataToSend, "%d", angle);
		HAL_UART_Transmit(shell.uartHandler, (uint8_t*)&dataToSend, 4, HAL_MAX_DELAY);
		break;
	default :
		printf("Unknown command has been called!\r\n");
	}
}


