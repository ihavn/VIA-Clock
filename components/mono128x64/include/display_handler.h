/*
 * display_handler.h
 *
 *  Created on: 28 Sep 2019
 *      Author: IHA
 */

#ifndef COMPONENTS_MONO128X64_INCLUDE_DISPLAY_HANDLER_H_
#define COMPONENTS_MONO128X64_INCLUDE_DISPLAY_HANDLER_H_
#include <u8g2.h>

#define DISPLAY_WIFI_SYMBOL_WIDTH 19
#define DISPLAY_WIFI_SYMBOL_HEIGHT 12

#define DISPLAY_BATTERY_SYMBOL_WIDTH  16
#define DISPLAY_BATTERY_SYMBOL_HEIGHT 9

void displayInit(void);
void displayPowerUp(void);
void displayPowerDown(void);
void displayWifiSymbol(uint8_t xPos, uint8_t yPos, uint8_t percentage);
void displayBatterySymbol(uint8_t xPos, uint8_t yPos, uint8_t percentage);
void displaySetFont(const uint8_t *font);
uint8_t displayString(uint8_t xPos, uint8_t yPos, char *string);


void displayTest(void);

#endif /* COMPONENTS_MONO128X64_INCLUDE_DISPLAY_HANDLER_H_ */
