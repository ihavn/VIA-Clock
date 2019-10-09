/* WiFi station Example

 This example code is in the Public Domain (or CC0 licensed, at your option.)

 Unless required by applicable law or agreed to in writing, this
 software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 CONDITIONS OF ANY KIND, either express or implied.
 */
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "task_wifi.h"

/* Uses WiFi configuration that you can set via 'make menuconfig'.

 If you'd rather not, just change the below entries to strings with
 the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
 */
#define ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define ESP_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY

/* FreeRTOS event group to signal when we are connected*/
EventGroupHandle_t wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;

static wifi_ap_record_t s_ap_record;
static wifi_config_t s_wifi_config = { .sta = { .ssid = ESP_WIFI_SSID, .password =
		ESP_WIFI_PASS }, };

static const char *TAG = "wifiTask";

static int s_retry_num = 0;

static esp_err_t event_handler(void *ctx, system_event_t *event) {
	switch (event->event_id) {
	case SYSTEM_EVENT_STA_START:
		esp_wifi_connect();
		break;
	case SYSTEM_EVENT_STA_GOT_IP:
		ESP_LOGI(TAG, "got ip:%s",
				ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
		s_retry_num = 0;
		xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
		break;
	case SYSTEM_EVENT_STA_DISCONNECTED: {
		if (s_retry_num < ESP_MAXIMUM_RETRY) {
			esp_wifi_connect();
			xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
			s_retry_num++;
			ESP_LOGI(TAG, "retry to connect to the AP");
		}
		ESP_LOGI(TAG, "connect to the AP fail\n");
		break;
	}
	default:
		break;
	}
	return ESP_OK;
}

esp_err_t wifiGetRSSI(int8_t * rssi)
{
	if (xEventGroupGetBits(wifi_event_group) & WIFI_CONNECTED_BIT)
	{
		esp_err_t err = esp_wifi_sta_get_ap_info(&s_ap_record);
		*rssi = s_ap_record.rssi;
		return err;
	}

	*rssi = -127;
	return WIFI_REASON_NO_AP_FOUND;
}

void wifiTask(void *pvParameter) {
	ESP_LOGI(TAG, "Wifi initialisation started");

	wifi_event_group = xEventGroupCreate();

	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &s_wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());


	ESP_LOGI(TAG, "Connect to ap SSID:%s password:%s", ESP_WIFI_SSID,
			ESP_WIFI_PASS);
	ESP_LOGI(TAG, "Wifi initialisation finished");


	int8_t rssi;

	wifiGetRSSI(&rssi);
	ESP_LOGI(TAG, "RSSI:%d",rssi);

	xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);

	for (;;) {
		//xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
		//ESP_LOGE(TAG, "Disconnected from AP!");
		// Let's try to connect after 10 minutes
		//vTaskDelay(600000L / portTICK_PERIOD_MS);
		//esp_wifi_connect();

		vTaskDelay(5000 / portTICK_PERIOD_MS);

		wifiGetRSSI(&rssi);
		ESP_LOGI(TAG, "RSSI:%d",rssi);
	}
}

