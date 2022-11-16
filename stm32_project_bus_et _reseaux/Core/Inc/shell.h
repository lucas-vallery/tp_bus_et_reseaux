/*
 * shell.h
 *
 *  Created on: Oct 22, 2022
 *      Author: lucas
 */

#ifndef INC_SHELL_H_
#define INC_SHELL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usart.h"
#include "BMP280.h"
#include "stepper.h"

#define BUFFER_SIZE 10
#define CMD_SIZE 5

typedef struct Shell{
	UART_HandleTypeDef* uartHandler;
	USART_TypeDef*      uartRegisters;
}Shell;

void shell_init(Shell* shell, UART_HandleTypeDef* uartHandler, USART_TypeDef* uartRegisters);

void shell_startRxIt(Shell* shell);

void shell_clearBuffer(char buffer[], int bufferSize);

void shell_charReceived(Shell* shell);

void shell_executeCmd();

#endif /* INC_SHELL_H_ */
