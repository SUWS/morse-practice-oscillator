/********************************************************************
 File Description:

 Contains main process for the morse sounder

********************************************************************/

#include <avr/io.h>
#include <string.h>
#include <stdio.h>

//Core definitions
#include "hardware.h"

//include delay.h after hardware.h due to F_CPU definition
#include <util/delay.h>

//core libraies
#include "uart.h"
#include "adc.h"
#include "i2c.h"
#include "MCP4725.h"
#include "led.h"
#include "tone_generator.h"
#include "keying.h"


int main(int argc, char const *argv[])
{
	ADCInit();
    UartInit();
    I2CInit();
    LEDInit();

    ToneInit();
    KeyingInit();

	// Setting PC2(dot) & PC3(dash) as digital inputs
	KEY_IN_PORT_DDR &= ~_BV(LED_DOT);
	KEY_IN_PORT_DDR &= ~_BV(LED_DASH);

	while(1)
	{
        //Check for slected mode
        if(((KEY_MODE_PORT_PIN >> KEY_MODE)&0x01)==1)
		{
			//Paddle key mode
			INDICATOR_LED_PORT |= _BV(INDICATOR_LED_GREEN);
			KeyingProcess();
		}
		else
        {
			//Straight key mode
	        INDICATOR_LED_PORT &= ~_BV(INDICATOR_LED_GREEN);

	        if(((KEY_IN_PORT_PIN >> KEY_STRAIGHT)&0x01)==1)
	        {
	            ToneStop();
	        }
	        else
	        {
	            if(!ToneEnabled())
	            {
	                ToneStart();
	            }
	        }
        }

        ADCProcess();

		//dot indicator LED
		if(!(KEY_IN_PORT_PIN & _BV(KEY_DOT)))
		      LEDDotOn();
		else
		      LEDDotOff();

		//dash indicator LED
		if(!(KEY_IN_PORT_PIN & _BV(KEY_DASH)))
		      LEDDashOn();
		else
		      LEDDashOff();

	}

	return 0;
}
