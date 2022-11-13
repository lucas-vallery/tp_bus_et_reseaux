/*
 * shell.h
 *
 *  Created on: Oct 20, 2022
 *      Author: jean-francoiscastellani
 */

#ifndef INC_SHELL_H_
#define INC_SHELL_H_

#include "usart.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 32
#define CMD_SIZE 5

extern UART_HandleTypeDef huart1;

void shell_startRxIt();
void shell_charReceived(char);
void shell_clearBuffer();

#endif /* INC_SHELL_H_ */
