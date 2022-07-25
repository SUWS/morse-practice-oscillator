/********************************************************************
 File Description:

 Contains hardware test program

********************************************************************/

#include <avr/io.h>
#include <string.h>
#include <stdio.h>

//Core definitions
#include "hardware.h"

#include "led.h"

//include delay.h after hardware.h due to F_CPU definition
#include <util/delay.h>


/**************************************/
/*! Converts and ADC reading into a dial position
 *
 * The dials are non linear so the values have been calculated based on observed readings.
 * \param val ADC value from the dial
 * \retval dial position from 0 to 10
 */
int adc_pos(int val)
{
	if(val<15) return 0;
	if(val<88) return 1;
	if(val<198)	return 2;
	if(val<314)	return 3;
	if(val<438) return 4;
	if(val<577) return 5;
	if(val<702) return 6;
	if(val<817) return 7;
	if(val<929) return 8;
	if(val<1010) return 9;
	return 10;
}

int main(int argc, char const *argv[])
{
    LEDInit();
    UartInit();

	// Setting PC2(dot) & PC3(dash) as digital inputs
	KEY_IN_PORT_DDR &= ~_BV(LED_DOT);
	KEY_IN_PORT_DDR &= ~_BV(LED_DASH);

    //
    ADMUX = (1<<REFS0);
    ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

	while(1)
	{
        UartPutString("Begining self test\r\n");

        //Check for slected mode
        if(((KEY_MODE_PORT_PIN >> KEY_MODE)&0x01)==1)
        {
            UartPutString("Mode: paddle\r\n");
        }
        else
        {
            UartPutString("Mode: straight\r\n");
        }

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



		//indicator red
		_delay_ms(1000);
		INDICATOR_LED_PORT &= ~_BV(INDICATOR_LED_RED);
		_delay_ms(1000);
		INDICATOR_LED_PORT |= _BV(INDICATOR_LED_RED);
        //indicator green
		_delay_ms(1000);
		INDICATOR_LED_PORT &= ~_BV(INDICATOR_LED_GREEN);
		_delay_ms(1000);
		INDICATOR_LED_PORT |= _BV(INDICATOR_LED_GREEN);
		//indicator blue
		_delay_ms(1000);
		INDICATOR_LED_PORT &= ~_BV(INDICATOR_LED_BLUE);
		_delay_ms(1000);
		INDICATOR_LED_PORT |= _BV(INDICATOR_LED_BLUE);


        //read adcs
        ADMUX = ADC_TONE;// represents PA2
        // start conversion
        ADCSRA |= _BV(ADSC);
        while(ADCSRA & (1 << ADSC));

        char tmp[24];
		sprintf(tmp,"Tone :%d (%d)\r\n",adc_pos(ADC),ADC);
		UartPutString(tmp);

        ADMUX = ADC_VOLUME;// represents PA2
        // start conversion
        ADCSRA |= _BV(ADSC);
        while(ADCSRA & (1 << ADSC));

		sprintf(tmp,"Volume :%d (%d)\r\n",adc_pos(ADC),ADC);
		UartPutString(tmp);

        ADMUX = ADC_RATE;// represents PA2
        // start conversion
        ADCSRA |= _BV(ADSC);
        while(ADCSRA & (1 << ADSC));

		sprintf(tmp,"Rate :%d (%d)\r\n",adc_pos(ADC),ADC);
		UartPutString(tmp);

        ADMUX = ADC_WEIGHT;// represents PA2
        // start conversion
        ADCSRA |= _BV(ADSC);
        while(ADCSRA & (1 << ADSC));

		sprintf(tmp,"Weight :%d (%d)\r\n",adc_pos(ADC),ADC);
		UartPutString(tmp);


	}

	return 0;
}
