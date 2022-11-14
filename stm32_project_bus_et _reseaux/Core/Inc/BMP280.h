/*
 * BMP280.h
 *
 *  Created on: Oct 19, 2022
 *      Author: jean-francoiscastellani
 */

#ifndef INC_BMP280_H_
#define INC_BMP280_H_

#include "main.h"
#include <stdio.h>

#define TEMP_XLSB 		0xFC
#define TEMP_LSB		0xFB
#define TEMP_MSB		0xFA
#define PRESS_XLSB		0xF9
#define PRESS_LSB		0xF8
#define PRESS_MSB 		0xF7
#define CONFIG			0xF5
#define CTRL_MEAS		0xF4
#define STATUS 			0xF3
#define RESET			0xE0
#define ID				0xD0
#define BMP_I2C_ADD 	0x77<<1
#define CALIB_TEMP_START		0x88
#define CALIB_PRESS_START		0x8E


extern I2C_HandleTypeDef hi2c2;

uint8_t BMP280_getId();
uint8_t BMP280_config();
float BMP280_readRawTemp();
float BMP280_readRawPress();
uint32_t BMP280_compensateTemp(uint8_t *etalonnage, uint32_t rawTemp);
uint8_t BMP280_Etalonnage(uint8_t* calibration);
uint32_t BMP280_compensatePress(uint8_t *calib, uint32_t rawPress);

#endif /* INC_BMP280_H_ */
