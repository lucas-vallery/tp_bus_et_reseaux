/*
 * shell.c
 *
 *  Created on: Oct 20, 2022
 *      Author: jean-francoiscastellani
 */

#include "shell.h"

int indexBuff = 0;
char charBuffer[BUFF_SIZE] = {0};
char charReceived = 0;
const char *tabCmd[]= {
		"GET_T",
		"GET_P",
		"SET_K",
		"GET_K",
		"GET_A"
};

void shell_startRxIt(){
	HAL_UART_Receive_IT(&huart2, (uint8_t*)&charReceived, 1);
}

void shell_charReceived(char charReceived){
	if (charReceived != '\r' || indexBuff<BUFF_SIZE){
		HAL_UART_Transmit(&huart2, (uint8_t*)&charReceived, 1, HAL_MAX_DELAY);
		charBuffer[indexBuff] = charReceived;
		indexBuff++;
	}
	else{
		shell_clearBuffer(charBuffer, BUFF_SIZE);
		indexBuff = 0;
	}
}

void shell_clearBuffer(char buffer[], int buffersize){
	int i;
	for (i = 0; i<buffersize; i++){
		buffer[i] = 0;
	}
}

uint8_t shell_exe(char* cmd){
	int indexCmd;
	int selectedCmd = -1;

	for (indexCmd = 0; indexCmd < CMD_SIZE; indexCmd++){
		if (strcmp(cmd, tabCmd[indexCmd])){
			selectedCmd = indexCmd;
			break;
		}
	}

	char bufferAnswer[32] = "";

	switch(selectedCmd){
		case 0:
			printf("\r\nT=%ld_C\r\n", BMP280_readRawTemp());
			/*
			snprintf(bufferAnswer, "\r\nT=%ld_C\r\n",(uint32_t)BMP280_readRawTemp());
			HAL_UART_Transmit(&huart2,(uint8_t*)bufferAnswer, 32, HAL_MAX_DELAY);
			*/
			break;
		default:
			printf("Unknown call");
	}

}








