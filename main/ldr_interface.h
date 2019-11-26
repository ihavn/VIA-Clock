/*
 * ldr_interface.h
 *
 *  Created on: 24 Nov 2019
 *      Author: IHA
 */

#ifndef MAIN_LDR_INTERFACE_H_
#define MAIN_LDR_INTERFACE_H_
#include "freertos/semphr.h"

void ldrInit(SemaphoreHandle_t semaphoreToGive);
bool ldrSeeLight(void);

#endif /* MAIN_LDR_INTERFACE_H_ */
