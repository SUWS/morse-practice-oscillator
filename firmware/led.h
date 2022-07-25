/********************************************************************
 File Description:

 definitions for the leds

********************************************************************/

#ifndef LED_H_
#define LED_H_

#include "stdint.h"

#define POWER_COLOUR_RED 0x01
#define POWER_COLOUR_GREEN 0x02
#define POWER_COLOUR_BLUE 0x04

#define POWER_COLOR_WHITE (POWER_COLOUR_RED | POWER_COLOUR_GREEN | POWER_COLOUR_BLUE)

/* FUNCTION DECLARATIONS *******************************************/

int LEDInit();

int UartPutString(char* str);

inline void LEDDotOn();
inline void LEDDotOff();
inline void LEDDashOn();
inline void LEDDashOff();

inline void LEDPowerSet(uint8_t colourCode);

#endif /* UART_H_ */
