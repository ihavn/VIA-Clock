/*
 * display_handler.h
 *
 *  Created on: 28 Sep 2019
 *      Author: IHA
 */

#ifndef COMPONENTS_MONO128X64_INCLUDE_DISPLAY_HANDLER_H_
#define COMPONENTS_MONO128X64_INCLUDE_DISPLAY_HANDLER_H_
void displayInit(void);
void displayPowerUp(void);
void displayPowerDown(void);
void displayWifiSymbol(uint8_t strength, uint8_t xPos, uint8_t yPos);



void displayTest(void);

#endif /* COMPONENTS_MONO128X64_INCLUDE_DISPLAY_HANDLER_H_ */
