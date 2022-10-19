/*
 * BMP280.c
 *
 *  Created on: Oct 19, 2022
 *      Author: jean-francoiscastellani
 */
#include "BMP280.h"


uint8_t BMP280_getId(){
	uint8_t id;
	uint8_t idRegister = (uint8_t)ID;
	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, I2C_ADD, idRegister, 1, HAL_MAX_DELAY)){
		printf("là");
		if (HAL_OK == HAL_I2C_Master_Receive(&hi2c1, I2C_ADD, &id, 1, HAL_MAX_DELAY)){
			printf("ici");
		}
	}

	return id;
}
