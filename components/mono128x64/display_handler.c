/*
 * display_handler.c
 *
 *  Created on: 29 Aug 2019
 *      Author: IHA
 */

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>
#include <u8g2.h>


#include "sdkconfig.h"
#include "u8g2_esp32_hal.h"

// SDA - PIN
#define PIN_SDA CONFIG_MONO128x64_I2C_SDA_PIN // 5 //21
// SCL - PIN
#define PIN_SCL CONFIG_MONO128x64_I2C_SCL_PIN // 4 // 22

static const char *TAG = "mono128x64";

static u8g2_esp32_hal_t _u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
static u8g2_t _u8g2; // a structure which will contain all the data for one display

void displayInit(void) {
	_u8g2_esp32_hal.sda = PIN_SDA;
	_u8g2_esp32_hal.scl = PIN_SCL;
	u8g2_esp32_hal_init(_u8g2_esp32_hal);

	u8g2_Setup_ssd1306_i2c_128x64_noname_f(&_u8g2, U8G2_R0,
			u8g2_esp32_i2c_byte_cb, u8g2_esp32_gpio_and_delay_cb); // init u8g2 structure

	u8x8_SetI2CAddress(&_u8g2.u8x8, 0x78);

	ESP_LOGI(TAG, "u8g2_InitDisplay");
	u8g2_InitDisplay(&_u8g2); // send init sequence to the display, display is in sleep mode after this,
}

void displayTest(void) {
	ESP_LOGI(TAG, "u8g2_SetPowerSave");
	u8g2_SetPowerSave(&_u8g2, 0); // wake up display

	ESP_LOGI(TAG, "u8g2_ClearBuffer");
	u8g2_ClearBuffer(&_u8g2);

	ESP_LOGI(TAG, "u8g2_DrawBox");
	u8g2_DrawBox(&_u8g2, 0, 26, 80, 6);
	u8g2_DrawFrame(&_u8g2, 0, 26, 100, 6);

	ESP_LOGI(TAG, "u8g2_SetFont");
	u8g2_SetFont(&_u8g2, u8g2_font_9x15B_mf);
	ESP_LOGI(TAG, "u8g2_DrawStr");
	u8g2_DrawStr(&_u8g2, 2, 15, "Header");
	ESP_LOGI(TAG, "u8g2_SendBuffer");
	u8g2_SendBuffer(&_u8g2);

	ESP_LOGI(TAG, "All done!");
}

void displayPowerUp(void)
{
	ESP_LOGI(TAG, "u8g2_SetPowerSave 0");
	u8g2_SetPowerSave(&_u8g2, 0); // power up display
}

void displayPowerDown(void)
{
	ESP_LOGI(TAG, "u8g2_SetPowerSave 1");
	u8g2_SetPowerSave(&_u8g2, 1); // power down display
}

void displayWifiSymbol(uint8_t strength, uint8_t xPos, uint8_t yPos)
{
	u8g2_SetFont(&_u8g2, u8g2_font_open_iconic_www_2x_t);
	u8g2_DrawGlyph(&_u8g2, 112, 15, 72);
	u8g2_SendBuffer(&_u8g2);
}
