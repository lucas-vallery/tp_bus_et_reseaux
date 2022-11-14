/*
 * stepper.c
 *
 *  Created on: Nov 14, 2022
 *      Author: jean-francoiscastellani
 */

#include "stepper.h"

uint32_t pTxMailBox;

uint8_t stepper_CanInit(stepper_t* stepper, CAN_HandleTypeDef * hcan){
	stepper->canHandler = hcan;
	stepper->K = 10;
	if (HAL_OK != HAL_CAN_Start(stepper->canHandler)){
		while(1);
	}
	return 0;
}

void stepper_set0(stepper_t* stepper){
	CAN_TxHeaderTypeDef pHeader;

	pHeader.StdId = 0x62;
	pHeader.ExtId = 0;
	pHeader.IDE = CAN_ID_STD;
	pHeader.RTR = CAN_RTR_DATA;
	pHeader.DLC = 0;
	pHeader.TransmitGlobalTime = DISABLE;

	if(HAL_CAN_AddTxMessage(stepper->canHandler, &pHeader, (uint8_t*)NULL, &pTxMailBox) != HAL_OK){
		Error_Handler();
	}
}

uint8_t stepper_setK(stepper_t* stepper, uint16_t K){
	stepper->K = K;
	return 0;
}

uint16_t stepper_getK(stepper_t* stepper){
	return stepper->K;
}

void stepper_WriteAngleSpeed(stepper_t* stepper, uint8_t angle, uint8_t sign, uint8_t speed){
	CAN_TxHeaderTypeDef pHeader;

	pHeader.StdId = 0x60;
	pHeader.ExtId = 0;
	pHeader.IDE = CAN_ID_STD;
	pHeader.RTR = CAN_RTR_DATA;
	pHeader.DLC = 3;
	pHeader.TransmitGlobalTime = DISABLE;

	uint8_t aData[3] = {angle, sign, speed};

	if(HAL_CAN_AddTxMessage(stepper->canHandler, &pHeader, (uint8_t*)&aData, &pTxMailBox) != HAL_OK){
		Error_Handler();
	}

}


