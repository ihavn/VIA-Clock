/* Home surveillance system
 *
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "display_handler.h"

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
	displayInit();
	//displayTest();
	displayPowerUp();
	for (;;) {
		for (int i = 0; i <= 10; i++) {
			printf("Im here!!\n");
			displayBatterySymbol(111, 3, i * 10 );
			displayWifiSymbol(0, 2, i*10);
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}

		for (int i = 10; i > 0; i--) {
			printf("And here!!\n");
			displayBatterySymbol(111, 3, i * 10 );
			displayWifiSymbol(0, 2, i*10);
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
	}

}
