/* Home surveillance system
 *
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_types.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/periph_ctrl.h"
#include "driver/gpio.h"
#include "driver/timer.h"

#include "handler_display.h"
#include "handler_wifi.h"

static const char *TAG = "MAIN";
static const int _stepperPattern[8][4] = { { 0, 1, 1, 0 }, { 1, 1, 1, 0 }, { 1,
		1, 0, 0 }, { 1, 1, 0, 1 }, { 1, 0, 0, 1 }, { 1, 0, 1, 1 },
		{ 0, 0, 1, 1 }, { 0, 1, 1, 1 } };

static void IRAM_ATTR timer_tg0_isr(void *para) {
	static int _step = 0;
	static uint16_t _counter = 0;

	int timer_idx = (int) para;

	/* Retrieve the interrupt status and the counter value
	 from the timer that reported the interrupt */
	uint32_t intr_status = TIMERG0.int_st_timers.val;
	TIMERG0.hw_timer[timer_idx].update = 1;

	/* Clear the interrupt*/
	if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_0) {
		TIMERG0.int_clr_timers.t0 = 1;
	}
	/* After the alarm has been triggered
	 we need enable it again, so it is triggered the next time */
	TIMERG0.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;

	if (_counter++ < 4076/60) {
	gpio_set_level(GPIO_NUM_16, _stepperPattern[_step][0]);
	gpio_set_level(GPIO_NUM_2, _stepperPattern[_step][1]);
	gpio_set_level(GPIO_NUM_14, _stepperPattern[_step][2]);
	gpio_set_level(GPIO_NUM_15, _stepperPattern[_step][3]);

	_step = (_step + 1) % 8;
	} else if (_counter > 500 ) {
		_counter = 0;
	}
}

static void example_tg0_timer_init(int timer_idx,
bool auto_reload, double timer_interval_us) {

	gpio_reset_pin(GPIO_NUM_16);
	gpio_reset_pin(GPIO_NUM_2);
	gpio_reset_pin(GPIO_NUM_14);
	gpio_reset_pin(GPIO_NUM_15);

	gpio_set_direction(GPIO_NUM_16, GPIO_MODE_OUTPUT);
	gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
	gpio_set_direction(GPIO_NUM_14, GPIO_MODE_OUTPUT);
	gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);

	/* Select and initialize basic parameters of the timer */
	timer_config_t config;
	config.divider = 800;
	config.counter_dir = TIMER_COUNT_UP;
	config.counter_en = TIMER_PAUSE;
	config.alarm_en = TIMER_ALARM_EN;
	config.intr_type = TIMER_INTR_LEVEL;
	config.auto_reload = auto_reload;
	timer_init(TIMER_GROUP_0, timer_idx, &config);

	/* Timer's counter will initially start from value below.
	 Also, if auto_reload is set, this value will be automatically reload on alarm */
	timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0x00000000ULL);

	/* Configure the alarm value and the interrupt on alarm. */
	timer_set_alarm_value(TIMER_GROUP_0, timer_idx, timer_interval_us);
	timer_enable_intr(TIMER_GROUP_0, timer_idx);
	timer_isr_register(TIMER_GROUP_0, timer_idx, timer_tg0_isr,
			(void*) timer_idx, ESP_INTR_FLAG_IRAM, NULL);

	timer_start(TIMER_GROUP_0, timer_idx);
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

// Needed by WIFI
	nvs_flash_init();

	u8g2_t _u8g2 = displayInit();
//displayTest();
	displayPowerUp();
	u8g2_SetFont(&_u8g2, u8g2_font_profont12_mr);

// Start WIFI task and let it connect to AP
	wifiInit();

	char ssid[20];
	wifiGetSSID(ssid);
	u8g2_DrawStr(&_u8g2, 5 + DISPLAY_WIFI_SYMBOL_WIDTH, 15, ssid);

	displayBatterySymbol(111, 3, 100);
	displayUpdate();

	uint8_t rssiPercent = 0;

	example_tg0_timer_init(TIMER_0, 1, 100);

	for (;;) {
		wifiGetRSSIPercent(&rssiPercent);
		displayWifiSymbol(0, 2, rssiPercent);
		displayUpdate();

		vTaskDelay(500 / portTICK_PERIOD_MS);
	}

}
