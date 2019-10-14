/*
 * handler_mqtt.h
 *
 *  Created on: 10 Oct 2019
 *      Author: IHA
 */

#ifndef COMPONENTS_HANDLER_MQTT_INCLUDE_HANDLER_MQTT_H_
#define COMPONENTS_HANDLER_MQTT_INCLUDE_HANDLER_MQTT_H_

#include "mqtt_client.h"

extern esp_mqtt_client_handle_t client;
void mqttInit(void);

#endif /* COMPONENTS_HANDLER_MQTT_INCLUDE_HANDLER_MQTT_H_ */
