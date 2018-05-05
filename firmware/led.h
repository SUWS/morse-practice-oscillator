/********************************************************************
 File Description:

 definitions for the leds

********************************************************************/

#ifndef LED_H_
#define LED_H_

/* FUNCTION DECLARATIONS *******************************************/

void LEDInit();

int UartPutString(char* str);

inline void LEDDotOn();
inline void LEDDotOff();
inline void LEDDashOn();
inline void LEDDashOff();

#endif /* UART_H_ */
