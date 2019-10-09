/*
 * task_wifi.h
 *
 *  Created on: 9 Oct 2019
 *      Author: IHA
 */

#ifndef TASKS_TASK_WIFI_H_
#define TASKS_TASK_WIFI_H_

#include "freertos/event_groups.h"

extern EventGroupHandle_t wifi_event_group;
/* The event group allows multiple bits for each event, but we only care about one event
 * - are we connected to the AP with an IP? */
extern const int WIFI_CONNECTED_BIT;

void wifiTask(void *pvParameter);
esp_err_t wifiGetRSSI(int8_t * rssi);

#endif /* TASKS_TASK_WIFI_H_ */
