/* Home surveillance system
 *
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "handler_display.h"
#include "handler_ds18b20.h"
#include "handler_wifi.h"
#include "handler_mqtt.h"

#include "ldr_interface.h"

#define pumpSamplePeriod  pdMS_TO_TICKS(1*60*1000) // 1 minute

static const char *TAG = "MAIN";
static SemaphoreHandle_t pumpSemaphore = NULL;

// Call back to timer that controls how often the pump data is sampled
static void pumpTimerCallback(TimerHandle_t xTimer) {
	xSemaphoreGive(pumpSemaphore);
}

static void initTimer(void) {
	TimerHandle_t timerHandle = xTimerCreate("PumpTimer", pumpSamplePeriod,
	pdTRUE, (void*) 0, pumpTimerCallback);

	xTimerStart(timerHandle, pdMS_TO_TICKS(500));
}

void app_main() {
	printf("Hello world!\n");

	/* Print chip information */
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);
	printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ", chip_info.cores,
			(chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
			(chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

	printf("silicon revision %d, ", chip_info.revision);

	printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
			(chip_info.features & CHIP_FEATURE_EMB_FLASH) ?
					"embedded" : "external");

	pumpSemaphore = xSemaphoreCreateBinary();
	ldrInit(pumpSemaphore);

// Needed by WIFI
	nvs_flash_init();

	u8g2_t _u8g2 = displayInit();
//displayTest();
	displayPowerUp();
	u8g2_SetFont(&_u8g2, u8g2_font_profont12_mr);

	char _tmp[30];

// Start WIFI task and let it connect to AP
	wifiInit();

// Start MQTT Client
	mqttInit();

// Draw battery symbol
	char ssid[20];
	wifiGetSSID(ssid);
	u8g2_DrawStr(&_u8g2, 5 + DISPLAY_WIFI_SYMBOL_WIDTH, 15, ssid);

	displayBatterySymbol(111, 3, 100);
	displayUpdate();

	ds18b20Init();
	int noOfTempSensors = ds18b20FindDevices();

	uint8_t rssiPercent = 0;

	char _topic[40];

	float results[DS18B20_MAX_DEVICES];
	DS18B20_ERROR errors[DS18B20_MAX_DEVICES] = { 0 };

	int _lastPumpState = -1;

	initTimer();

	for (;;) {
		ds18b20GetAllTemperatures(results, errors);

		for (int i = 0; i < noOfTempSensors; i++) {
			sprintf(_tmp, "T%d:%4.1f", i, results[i]);
			u8g2_DrawStr(&_u8g2, 0, 30 + (i * 11), _tmp);

			sprintf(_topic, "/pump/temperature/%s/",
					ds18b20GetDeviceSerialNumber(i));
			sprintf(_tmp, "%4.1f", results[i]);
			esp_mqtt_client_publish(client, _topic, _tmp, 0, 0, 0);
		}

		int _ldr = ldrSeeLight();
		if (_lastPumpState != _ldr) {
			sprintf(_topic, "/pump/state/");
			sprintf(_tmp, "%d", ldrSeeLight());
			esp_mqtt_client_publish(client, _topic, _tmp, 0, 0, 0);

			sprintf(_tmp, "Pump: %s", (_ldr ? "ON " : "OFF"));
			u8g2_DrawStr(&_u8g2, 70, 30, _tmp);

			_lastPumpState = _ldr;
		}

		wifiGetRSSIPercent(&rssiPercent);
		displayWifiSymbol(0, 2, rssiPercent);
		displayUpdate();

		xSemaphoreTake(pumpSemaphore, portMAX_DELAY);
	}
}
