/*
 * handler_ds18b20.h
 *
 *  Created on: 8 Oct 2019
 *      Author: IHA
 */

#ifndef COMPONENTS_HANDLER_DS18B20_INCLUDE_HANDLER_DS18B20_H_
#define COMPONENTS_HANDLER_DS18B20_INCLUDE_HANDLER_DS18B20_H_

#include "ds18b20.h"

#define DS18B20_MAX_DEVICES          (8)

void ds18b20Init(void);
void ds18b20Destroy(void);
int ds18b20FindDevices(void);
float ds18b20GetSingleTemperature(int device);

void ds18b20GetAllTemperatures(float results[DS18B20_MAX_DEVICES], DS18B20_ERROR errors[DS18B20_MAX_DEVICES]);
char * ds18b20GetDeviceSerialNumber(int deviceNumber );

#endif /* COMPONENTS_HANDLER_DS18B20_INCLUDE_HANDLER_DS18B20_H_ */
