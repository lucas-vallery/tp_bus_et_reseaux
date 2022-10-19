/*
 * BMP280.h
 *
 *  Created on: Oct 19, 2022
 *      Author: jean-francoiscastellani
 */

#ifndef INC_BMP280_H_
#define INC_BMP280_H_

#include "main.h"

#define ID 0xD0
#define RESET 0xE0
#define I2C_ADD 0x77
#define CTRL_MEAS 0xF4
#define TEMP_XLSB 0xFC
#define TEMP_LSB 0xFB
#define TEMP_MSB 0xFA
#define PRESS_XLSB 0xF9
#define PRESS_LSB 0xF8
#define PRESS_MSB 0xF7

extern I2C_HandleTypeDef hi2c1;

uint8_t BMP280_getId();

#endif /* INC_BMP280_H_ */
