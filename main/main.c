/* Home surveillance system
 *
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "nvs_flash.h"

#include "../components/handler_oled_128x64/include/handler_display.h"
#include "handler_ds18b20.h"
#include "tasks/task_wifi.h"

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

    nvs_flash_init();


	u8g2_t _u8g2 = displayInit();
	//displayTest();
	displayPowerUp();
	u8g2_SetFont(&_u8g2, u8g2_font_profont12_mr);

	char _tmp[30];

	for (int i = 0; i <= 2; i++) {
		printf("Im here!!\n");
		sprintf(_tmp, "%5d %%", i * 10);
		u8g2_DrawStr(&_u8g2, 50, 15, _tmp);
		displayBatterySymbol(111, 3, i * 10);
		displayWifiSymbol(0, 2, i * 10);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

	for (int i = 2; i > 0; i--) {
		printf("And here!!\n");
		sprintf(_tmp, "%5d %%", i * 10);
		u8g2_DrawStr(&_u8g2,50, 15, _tmp);
		displayBatterySymbol(111, 3, i * 10);
		displayWifiSymbol(0, 2, i * 10);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

	// Start WIFI task and let it connect to AP
	xTaskCreate(&wifiTask, "task_wifi", 2096, NULL, 5, NULL);

	ds18b20Init();
	int noOfTemp = ds18b20FindDevices();

	for (;;) {
//		for (int i = 0; i < noOfTemp; i++) {
//			printf("T%d: %5.1f\n", i, ds18b20GetSingleTemperature(i));
//			vTaskDelay(1000 / portTICK_PERIOD_MS);
//		}

		float results[DS18B20_MAX_DEVICES];
		DS18B20_ERROR errors[DS18B20_MAX_DEVICES] = { 0 };

		ds18b20GetAllTemperatures(results, errors);
		for (int i = 0; i < noOfTemp; i++) {
			sprintf(_tmp, "T%d:%4.1f", i, results[i]);
			printf("%s\n",_tmp);
			u8g2_DrawStr(&_u8g2, 0, 30+(i*11), _tmp);
			displayUpdate();
		}
	}
}
