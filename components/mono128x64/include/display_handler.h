/*
 * display_handler.h
 *
 *  Created on: 28 Sep 2019
 *      Author: IHA
 */

#ifndef COMPONENTS_MONO128X64_INCLUDE_DISPLAY_HANDLER_H_
#define COMPONENTS_MONO128X64_INCLUDE_DISPLAY_HANDLER_H_
#define DISPLAY_WIFI_SYMBOL_WIDTH 19
#define DISPLAY_WIFI_SYMBOL_HEIGHT 12
void displayInit(void);
void displayPowerUp(void);
void displayPowerDown(void);
void displayWifiSymbol(uint8_t percent, uint8_t xPos, uint8_t yPos);



void displayTest(void);

#endif /* COMPONENTS_MONO128X64_INCLUDE_DISPLAY_HANDLER_H_ */
