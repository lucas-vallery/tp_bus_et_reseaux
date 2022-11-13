/*
 * shell.c
 *
 *  Created on: 20 oct. 2022
 *      Author: lucas
 */
#include "shell.h"

int indexBuff = 0;
char charBuffer[BUFF_SIZE] = {'\0'};
extern char lastChar;

const char *tabCmd[]= {
		"GET_T",
		"GET_P",
		"SET_K",
		"GET_K",
		"GET_A"
};


void shell_clearBuffer(char buffer[], int bufferSize){
	int i;
	for(i = 0; i < bufferSize; i++) {
		buffer[i] = '\0';
	}
}

void shell_startRxIt(){
	HAL_UART_Receive_IT(&huart1, (uint8_t*) &lastChar, 1);
}

void shell_charReceived(char charReceived) {
	if(charReceived != '\r' || indexBuff < BUFF_SIZE){
		HAL_UART_Transmit(&huart1, (uint8_t*) &charReceived, 1, HAL_MAX_DELAY);
		charBuffer[indexBuff] = charReceived;
	} else {
		indexBuff = 0;
		printf("here");
		shell_execute(charBuffer);
		shell_clearBuffer(charBuffer, BUFF_SIZE);
	}
}

uint8_t shell_execute(char* cmd){
	int indexCmd;
	int selectedCmd = -1;

	for(indexCmd = 0; indexCmd < CMD_SIZE; indexCmd++) {
		if(!strcmp(cmd, tabCmd[indexCmd])){
			selectedCmd = indexCmd;
		}
	}


	switch(selectedCmd){
	case 0 :
		printf("\r\nT=12\r\n");
		/*
		snprintf(answerBuffer, "\r\nT=%ld\r\n", BMP280_readRawTemp());
		HAL_UART_Transmit(&huart1, (uint8_t*) &answerBuffer, BUFF_SIZE, HAL_MAX_DELAY);
		 */
		break;

	default :
		printf("Unknown call");
	}

	return 0;
}




