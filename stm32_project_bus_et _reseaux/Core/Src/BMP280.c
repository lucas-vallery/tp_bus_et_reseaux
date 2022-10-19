/*
 * BMP280.c
 *
 *  Created on: Oct 19, 2022
 *      Author: jean-francoiscastellani
 */
#include "BMP280.h"


uint8_t BMP280_getId(){
	uint8_t id;

	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, I2C_ADD, ID, 1, HAL_MAX_DELAY)){
		if (HAL_OK == HAL_I2C_Master_Receive(&hi2c1, I2C_ADD, &id, 1, HAL_MAX_DELAY)){

		}
	}

	return id;
}
