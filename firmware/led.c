/********************************************************************
 File Description:

 Contains code for controling the LEDs

********************************************************************/

#include "led.h"
#include <avr/io.h>

//Core definitions
#include "hardware.h"
#include "error_codes.h"

int LEDInit()
{
    KEY_LED_PORT_DDR |= _BV(LED_DOT) + _BV(LED_DASH);   //set paddle input leds as outputs
    KEY_LED_PORT |= _BV(LED_DOT) + _BV(LED_DASH);       //turn off the leds by default

    INDICATOR_LED_DDR |= _BV(INDICATOR_LED_RED) + _BV(INDICATOR_LED_BLUE) + _BV(INDICATOR_LED_GREEN);   //set RGB status led as output
    INDICATOR_LED_PORT |= _BV(INDICATOR_LED_RED) + _BV(INDICATOR_LED_BLUE) + _BV(INDICATOR_LED_GREEN);   //set RGB status led to off
    return SUCCESS;
}

inline void LEDDotOn()
{
    KEY_LED_PORT &= ~_BV(LED_DOT);
}
inline void LEDDotOff()
{
    KEY_LED_PORT |= _BV(LED_DOT);
}

inline void LEDDashOn()
{
    KEY_LED_PORT &= ~_BV(LED_DASH);
}
inline void LEDDashOff()
{
    KEY_LED_PORT |= _BV(LED_DASH);
}

inline void LEDPowerSet(uint8_t colourCode)
{
    if(colourCode & POWER_COLOUR_RED)
    {
        INDICATOR_LED_PORT &= ~_BV(INDICATOR_LED_RED);
    }
    else
    {
        INDICATOR_LED_PORT |= _BV(INDICATOR_LED_RED);
    }

    if(colourCode & POWER_COLOUR_GREEN)
    {
        INDICATOR_LED_PORT &= ~_BV(INDICATOR_LED_GREEN);
    }
    else
    {
        INDICATOR_LED_PORT |= _BV(INDICATOR_LED_GREEN);
    }

    if(colourCode & POWER_COLOUR_BLUE)
    {
        INDICATOR_LED_PORT &= ~_BV(INDICATOR_LED_BLUE);
    }
    else
    {
        INDICATOR_LED_PORT |= _BV(INDICATOR_LED_BLUE);
    }

}
