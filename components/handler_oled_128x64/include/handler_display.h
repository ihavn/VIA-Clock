/**
\file
\brief Handler for small monochrome OLED displays.

The handler uses olikraus fantastic U8G2 library.
This is just a little supplement to he's library that can be found here: https://github.com/olikraus/u8g2.
All the functions in olikraus library can of course be used together with this handler.

\note The display will only be update when \ref displayUpdate or the original U8G2 function u8g2_SendBuffer is called.

\author Ib Havn

\defgroup oled_handler Handler for OLED.
\{
\brief Handler for small monochrome OLED displays.

\defgroup oled_creation Functions to create and initialise the handler.
\brief How to create the handler.

\defgroup oled_function Handler functions
\brief Commonly used functions.
Here you you will find the functions you normally will need.

\defgroup oled_return_codes OLED Handler Return codes
\brief Return codes from handler functions.
\}
*/

#ifndef COMPONENTS_HANDLER_OLED_128X64_INCLUDE_HANDLER_DISPLAY_H_
#define COMPONENTS_HANDLER_OLED_128X64_INCLUDE_HANDLER_DISPLAY_H_

#include <u8g2.h>

/** Total width in pixels of the WIFI-symbol */
#define DISPLAY_WIFI_SYMBOL_WIDTH 19

/** Total height in pixels of the WIFI-symbol */
#define DISPLAY_WIFI_SYMBOL_HEIGHT 12

/** Total width in pixels of the Battery-symbol */
#define DISPLAY_BATTERY_SYMBOL_WIDTH  16

/** Total height in pixels of the Battery-symbol */
#define DISPLAY_BATTERY_SYMBOL_HEIGHT 9

/* ======================================================================================================================= */
/**
\ingroup oled_creation
\brief Create the handler.

Creates and initialise the OLED Handler.

\note Must be called before any other handler or U8G2 functions are called!

\return the u8g2_t instance. This must be used as the first parameter to many of the original U8G2 functions.
*/
u8g2_t displayInit(void);

/* ======================================================================================================================= */
/**
\ingroup oled_function
\brief Powers up the OLED display.
*/
void displayPowerUp(void);

/* ======================================================================================================================= */
/**
\ingroup oled_function
\brief Powers down the OLED display.
*/
void displayPowerDown(void);

/* ======================================================================================================================= */
/**
\ingroup oled_function
\brief Display/Update the WIFI Symbol.

\param[in] xPos The X-position in pixel for the symbol.
\param[in] yPos The Y-position in pixel for the lowest pixel in symbol.
\param[in] percentage The signal strength in percents [0..100] to show.
*/
void displayWifiSymbol(uint8_t xPos, uint8_t yPos, uint8_t percentage);

/* ======================================================================================================================= */
/**
\ingroup oled_function
\brief Display/Update the Battery Symbol.

\param[in] xPos The X-position in pixel for the symbol.
\param[in] yPos The Y-position in pixel for the lowest pixel in symbol.
\param[in] percentage The battery level in percents [0..100] to show.
*/
void displayBatterySymbol(uint8_t xPos, uint8_t yPos, uint8_t percentage);

/* ======================================================================================================================= */
/**
\ingroup oled_function
\brief Send the display buffer to the OLED display.

\note Must be executed each time the display should show the latest updates in the display buffer.

\note This is just a wrapper for the original u8g2_SendBuffer function.
*/
void displayUpdate(void);

/* ======================================================================================================================= */
/**
\ingroup oled_function
\brief Just for test purpose - will be removed!

\todo Remove when finished!
*/
void displayTest(void);

#endif /* COMPONENTS_HANDLER_OLED_128X64_INCLUDE_HANDLER_DISPLAY_H_ */
