/*
 * shell.h
 *
 *  Created on: 20 oct. 2022
 *      Author: lucas
 */

#ifndef INC_SHELL_H_
#define INC_SHELL_H_

#include <string.h>
#include <stdio.h>

#include "usart.h"
#include"BMP280.h"

#define BUFF_SIZE 32
#define CMD_SIZE 5

extern UART_HandleTypeDef huart1;

void shell_clearBuffer(char buffer[], int bufferSize);
void shell_startRxIt();
void shell_charReceived(char charReceived);
uint8_t shell_execute(char* cmd);

#endif /* INC_SHELL_H_ */
