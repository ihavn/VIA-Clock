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

#include <display_handler.h>

// SDA - PIN
#define PIN_SDA CONFIG_MONO128x64_I2C_SDA_PIN // 5 //21
// SCL - PIN
#define PIN_SCL CONFIG_MONO128x64_I2C_SCL_PIN // 4 // 22

static const char *TAG = "mono128x64";

static u8g2_esp32_hal_t _u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
static u8g2_t _u8g2; // a structure which will contain all the data for one display

// -------------------------------------------------------------------------
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

// -------------------------------------------------------------------------
void displayTest(void) {
	ESP_LOGI(TAG, "u8g2_ClearBuffer");
	u8g2_ClearBuffer(&_u8g2);

	ESP_LOGI(TAG, "u8g2_DrawBox");
	u8g2_DrawBox(&_u8g2, 0, 26, 80, 6);
	u8g2_DrawFrame(&_u8g2, 0, 26, 100, 6);

	/*	ESP_LOGI(TAG, "u8g2_SetFont");
	 u8g2_SetFont(&_u8g2, u8g2_font_9x15B_mf);
	 ESP_LOGI(TAG, "u8g2_DrawStr");
	 u8g2_DrawStr(&_u8g2, 2, 15, "Header");
	 */
	ESP_LOGI(TAG, "u8g2_SendBuffer");
	u8g2_SendBuffer(&_u8g2);

	ESP_LOGI(TAG, "All done!");
}

// -------------------------------------------------------------------------
void displayPowerUp(void) {
	ESP_LOGI(TAG, "u8g2_SetPowerSave 0");
	u8g2_SetPowerSave(&_u8g2, 0); // power up display
}

// -------------------------------------------------------------------------
void displayPowerDown(void) {
	ESP_LOGI(TAG, "u8g2_SetPowerSave 1");
	u8g2_SetPowerSave(&_u8g2, 1); // power down display
}

// -------------------------------------------------------------------------
void displayWifiSymbol(uint8_t xPos, uint8_t yPos, uint8_t percentage) {

	static uint8_t _lastCol = 200;
	// wifi contour
	static unsigned char image_bits[] = { 0x00, 0x00, 0xff, 0x00, 0x00, 0xfd,
			0x00, 0x70, 0xfd, 0x00, 0x50, 0xfd, 0x00, 0x57, 0xfd, 0x00, 0x55,
			0xfd, 0x70, 0x55, 0xfd, 0x50, 0x55, 0xfd, 0x57, 0x55, 0xfd, 0x55,
			0x55, 0xfd, 0x55, 0x55, 0xfd, 0x77, 0x77, 0xff };

	if (200 == _lastCol) // First time draw contour
			{
		u8g2_DrawXBM(&_u8g2, xPos, yPos, DISPLAY_WIFI_SYMBOL_WIDTH, DISPLAY_WIFI_SYMBOL_HEIGHT, image_bits);
		_lastCol = 0;
	}

	// test if valid percentage
	if (percentage > 100) {
		percentage = 100;
	}

	uint8_t _col = 0;

	if (percentage > 1) {
		_col = 1 + (percentage - 1) / 20;

		if (_col > _lastCol) {
			for (uint8_t c = _lastCol; c < _col; c++) {
				u8g2_DrawBox(&_u8g2, (xPos + 1) + (c * 4), yPos + 1 + ((4 - c) * 2),
						1, 2 + (c * 2));
			}
		} else if (_col < _lastCol) {
			u8g2_SetDrawColor(&_u8g2, 0);
			for (uint8_t c = _col; c < 5; c++) {
				u8g2_DrawBox(&_u8g2, (xPos + 1) + (c * 4), yPos + 1 + ((4 - c) * 2),
						1, 2 + (c * 2));
			}
			u8g2_SetDrawColor(&_u8g2, 1);
		}
	} else {
		// clear all
		u8g2_DrawXBM(&_u8g2, xPos, yPos, DISPLAY_WIFI_SYMBOL_WIDTH, DISPLAY_WIFI_SYMBOL_HEIGHT, image_bits);
	}

	// only draw if there are changes
	if (_col != _lastCol) {
		u8g2_SendBuffer(&_u8g2);
		_lastCol = _col;
	}
}

// -------------------------------------------------------------------------
void displayBatterySymbol(uint8_t xPos, uint8_t yPos, uint8_t percentage) {


	static uint8_t _lastWidth = 200;
	// battery contour
	const unsigned char imageBits[] = { 0xff, 0x3f, 0xff, 0x3f, 0x03, 0xf0,
			0x03, 0xf0, 0x03, 0xf0, 0x03, 0xf0, 0x03, 0xf0, 0xff, 0x3f, 0xff,
			0x3f };

	// First time draw contour
	if (200 == _lastWidth) {
		u8g2_DrawXBM(&_u8g2, xPos, yPos, DISPLAY_BATTERY_SYMBOL_WIDTH, DISPLAY_BATTERY_SYMBOL_HEIGHT,
				imageBits);
		_lastWidth = 0;
	}

	// test if valid percentage
	if (percentage > 100) {
		percentage = 100;
	}

	uint8_t _width = (percentage + 5) / 10;

	if (_width > _lastWidth) {
		u8g2_DrawBox(&_u8g2, xPos + 2, yPos + 2, _width, 5);
	} else if (_width < _lastWidth) {
		u8g2_SetDrawColor(&_u8g2, 0);
		u8g2_DrawBox(&_u8g2, xPos + 2 + _width, yPos + 2, 10 - _width, 5);
		u8g2_SetDrawColor(&_u8g2, 1);
	}

	// only draw if there are changes
	if (_width != _lastWidth) {
		u8g2_SendBuffer(&_u8g2);
		_lastWidth = _width;
	}
}

// -------------------------------------------------------------------------
void displaySetFont(const uint8_t *font)
{
	u8g2_SetFont(&_u8g2, font);
}

// -------------------------------------------------------------------------
uint8_t displayString(uint8_t xPos, uint8_t yPos, char *string)
{
	uint8_t _width = u8g2_DrawStr(&_u8g2, xPos, yPos, string);
	u8g2_SendBuffer(&_u8g2);

	return _width;
}
