/*
 * BMP280.c
 *
 *  Created on: Oct 19, 2022
 *      Author: jean-francoiscastellani
 */
#include "BMP280.h"


uint8_t BMP280_getId(){
	uint8_t id_register = (uint8_t)ID;
	uint8_t id_rx_value;
	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c2, BMP_I2C_ADD, &id_register, 1, HAL_MAX_DELAY)){
		if(HAL_OK == HAL_I2C_Master_Receive(&hi2c2, BMP_I2C_ADD, &id_rx_value, 1, HAL_MAX_DELAY)){

		}
	}
	return id_rx_value;
}

uint8_t BMP280_config(){
	uint8_t ctrl_meas_register = (uint8_t)CTRL_MEAS;
	uint8_t ctrl_meas_init_value = 0b01010111;

	uint8_t tx_frame[2] = {ctrl_meas_register, ctrl_meas_init_value};
	uint8_t ctrl_meas_rx_value;

	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c2, BMP_I2C_ADD, tx_frame, 2, HAL_MAX_DELAY)){
		if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c2, BMP_I2C_ADD, &ctrl_meas_register, 1, HAL_MAX_DELAY)){
			if(HAL_OK == HAL_I2C_Master_Receive(&hi2c2, BMP_I2C_ADD, &ctrl_meas_rx_value, 1, HAL_MAX_DELAY)){

			}
		}
	}
	return ctrl_meas_rx_value;
}

uint8_t BMP280_Etalonnage(uint8_t* calibration){
	uint8_t etalonnage_register = (uint8_t)CALIB_TEMP_START;
	for (int i = 0; i<26; i++){
		if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c2, BMP_I2C_ADD, &etalonnage_register, 1, HAL_MAX_DELAY)){
			if(HAL_OK == HAL_I2C_Master_Receive(&hi2c2, BMP_I2C_ADD, calibration, 26, HAL_MAX_DELAY)){

			}
		}
	}
	return 0;
}

uint32_t BMP280_readRawTemp(){
	uint8_t temp_msb_register = (uint8_t)TEMP_MSB;
	uint8_t temp_frame_rx[3] = {0};

	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c2, BMP_I2C_ADD, &temp_msb_register, 1, HAL_MAX_DELAY)){
		if(HAL_OK == HAL_I2C_Master_Receive(&hi2c2, BMP_I2C_ADD, temp_frame_rx, 3, HAL_MAX_DELAY)){

		}
	}
	return (temp_frame_rx[0]<<12) | (temp_frame_rx[1]<<4) | (temp_frame_rx[2]>>4);
}

float BMP280_readCompensateTemp(){
	uint8_t temp_msb_register = (uint8_t)TEMP_MSB;
	uint32_t rawTemp = BMP280_readRawTemp();

	uint8_t tempArray[6] = {0};
	temp_msb_register = (uint8_t)CALIB_TEMP_START;

	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c2, BMP_I2C_ADD, &temp_msb_register, 1, HAL_MAX_DELAY)){
		if(HAL_OK == HAL_I2C_Master_Receive(&hi2c2, BMP_I2C_ADD, tempArray, 6, HAL_MAX_DELAY)){

		}
	}

	return (float)BMP280_compensateTemp(tempArray, rawTemp)/100;
}

uint32_t BMP280_compensateTemp(uint8_t *calib, uint32_t rawTemp){
	uint32_t dig_T1 = calib[0] | calib[1]<<8;
	uint32_t dig_T2 = calib[2] | calib[3]<<8;
	uint32_t dig_T3	= calib[4] | calib[5]<<8;

	uint32_t var1 = ((((rawTemp >> 3) - (dig_T1 << 1)))
			* dig_T2) >> 11;
	uint32_t var2 = (((((rawTemp >> 4) - dig_T1)
			* ((rawTemp >> 4) - dig_T1)) >> 12)
			* dig_T3) >> 14;

	uint32_t t_fine = var1 + var2;

	return (t_fine * 5 + 128) >> 8;
}

uint32_t BMP280_readRawPress(){
	uint8_t press_frame_rx[3] = {0};
	uint8_t press_msb_register = (uint8_t)PRESS_MSB;

	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c2, BMP_I2C_ADD, &press_msb_register, 1, HAL_MAX_DELAY)){
		if(HAL_OK == HAL_I2C_Master_Receive(&hi2c2, BMP_I2C_ADD, press_frame_rx, 3, HAL_MAX_DELAY)){

		}
	}

	return (press_frame_rx[0]<<12) | (press_frame_rx[1]<<4) | (press_frame_rx[2]>>4);
}

float BMP280_readCompensatePress(){
	uint8_t press_msb_register = (uint8_t)PRESS_MSB;
	uint32_t rawPress = BMP280_readRawPress();

	uint8_t pressArray[18] = {0};
	press_msb_register = (uint8_t)CALIB_PRESS_START;

	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c2, BMP_I2C_ADD, &press_msb_register, 1, HAL_MAX_DELAY)){
		if(HAL_OK == HAL_I2C_Master_Receive(&hi2c2, BMP_I2C_ADD, pressArray, 6, HAL_MAX_DELAY)){

		}
	}

	return BMP280_compensatePress(pressArray, rawPress)/256.0;
}

uint32_t BMP280_compensatePress(uint8_t *calib, uint32_t rawPress) {
	uint32_t dig_P1 = calib[0] | calib[1]<<8;
	uint32_t dig_P2 = calib[2] | calib[3]<<8;
	uint32_t dig_P3	= calib[4] | calib[5]<<8;
	uint32_t dig_P4	= calib[6] | calib[7]<<8;
	uint32_t dig_P5	= calib[8] | calib[9]<<8;
	uint32_t dig_P6	= calib[10] | calib[11]<<8;
	uint32_t dig_P7	= calib[12] | calib[13]<<8;
	uint32_t dig_P8	= calib[14] | calib[15]<<8;
	uint32_t dig_P9	= calib[16] | calib[17]<<8;

	uint32_t t_fine = 0;
	uint64_t var1, var2, p;

	var1 = ((uint64_t)t_fine) - 128000;
	var2 = var1 * var1 * (uint64_t)dig_P6;
	var2 = var2 + ((var1*(uint64_t)dig_P5)<<17);
	var2 = var2 + (((uint64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (uint64_t)dig_P3)>>8) + ((var1 * (uint64_t)dig_P2)<<12);
	var1 = (((((uint64_t)1)<<47)+var1))*((uint64_t)dig_P1)>>33;
	if(var1 == 0) {
		return 0;
	}
	p = 1048576-rawPress;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((uint64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((uint64_t)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((uint64_t)dig_P7)<<4);
	return (uint32_t)p;
}
