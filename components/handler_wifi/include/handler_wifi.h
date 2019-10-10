/*
 * handler_wifi.h
 *
 *  Created on: 9 Oct 2019
 *      Author: IHA
 */

#ifndef COMPONENTS_HANDLER_WIFI_INCLUDE_HANDLER_WIFI_H_
#define COMPONENTS_HANDLER_WIFI_INCLUDE_HANDLER_WIFI_H_

#include "freertos/event_groups.h"

extern EventGroupHandle_t wifi_event_group;
/* The event group allows multiple bits for each event, but we only care about one event
 * - are we connected to the AP with an IP? */
extern int WIFI_CONNECTED_BIT;

void wifiInit(void);
esp_err_t wifiGetRSSI(int8_t * rssi);
esp_err_t wifiGetRSSIPercent(uint8_t * rssiPercent);
void wifiGetSSID(char * ssid);

#endif /* COMPONENTS_HANDLER_WIFI_INCLUDE_HANDLER_WIFI_H_ */
