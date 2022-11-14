/*
 * stepper.h
 *
 *  Created on: Nov 14, 2022
 *      Author: jean-francoiscastellani
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "can.h"

typedef struct stepper_struct{
	CAN_HandleTypeDef* canHandler;
	uint16_t K;

}stepper_t;


uint8_t stepper_CanInit(stepper_t* stepper, CAN_HandleTypeDef * hcan);
void stepper_WriteAngleSpeed(stepper_t* stepper, uint8_t angle, uint8_t sign, uint8_t speed);
void stepper_set0(stepper_t* stepper);
uint8_t stepper_setK(stepper_t* stepper, uint16_t K);
uint16_t stepper_getK(stepper_t* stepper);

#endif /* INC_STEPPER_H_ */
