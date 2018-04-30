// FOR DIGITAL/ANALOGE INPUTS:
// 1.)  Need to digitally read the "dat" padel(ADC_DAT) or "dah" padel (ADC_DASH)
// 2.) You are also trying to read the weight of the PWM signals (duty cycle) that used to convey the morse code
// using ADC_WEIGHT pin (PC0).
// 3.) Also the speed of the morse code is also read as an analogue voltage
// through ADC_RATE pin (PC1)
// FFOR DIGITAL OUTPUTS:
// 1.) We output a string of "dat" or "dah" though AVR_KEY_OUT (PC4)
// when either the "dat" or "dah" key/padel is held on continuously

//******REFERENCE**********
// [1] ADC Initialization section from URL: http://maxembedded.com/2011/06/the-adc-of-the-avr/
// [2] Reading ADC Value section form URL : http://maxembedded.com/2011/06/the-adc-of-the-avr/
//*************************


#include <avr/io.h>

//Core definitions
#include "hardware.h"

//include delay.h after hardware.h due to F_CPU definition
#include <util/delay.h>


void adc_init()// [1]
{
    // AREF = AVcc ie. select Vcc as reference voltage by setting REFS1:0 = 01
    ADMUX = (1<<REFS0);

    // ADC Enable by setting ADEN
    //and prescaler of 128 by setting ADP2:0 to 1
    // ie. 16000000/128 = 125000
    ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

// adc_read() to read weight and rate analogue signal
uint16_t adc_read(uint8_t ch)// [2]
{
	ADMUX = ch;// represents PA2
	// start conversion
	ADCSRA |= _BV(ADSC);
	// wait for conversion to complete
	//while(!(ADCSRA & _BV(ADIF))){};
	while(ADCSRA & _BV(ADSC));
	ADC = (ADCH << 8) | ADCL;// [1]
	return ADC;
}


int main(int argc, char const *argv[])
{
	// Setting PC2(dot) & PC3(dash) as digital inputs
	KEY_IN_PORT_DDR &= ~_BV(LED_DOT);
	KEY_IN_PORT_DDR &= ~_BV(LED_DASH);

	// set LEDs as OUTPUTS
	KEY_LED_PORT_DDR |= _BV(LED_DOT) + _BV(LED_DASH);
	INDICATOR_LED_DDR |= _BV(INDICATOR_LED_RED) + _BV(INDICATOR_LED_BLUE) + _BV(INDICATOR_LED_GREEN);

	while(1)
	{
		//dot indicator LED
		//if((KEY_IN_PORT_PIN & _BV(KEY_DOT)))
		_delay_ms(1000);
		KEY_LED_PORT |= _BV(LED_DOT);
		//else
		_delay_ms(1000);
		KEY_LED_PORT &= ~_BV(LED_DOT);

		//dash indicator LED
		//if((KEY_IN_PORT_PIN & _BV(KEY_DASH)))
		_delay_ms(1000);
		KEY_LED_PORT |= _BV(LED_DASH);
		_delay_ms(1000);
		//else
		KEY_LED_PORT &= ~_BV(LED_DASH);


		//indicator red
		_delay_ms(1000);
		INDICATOR_LED_PORT |= _BV(INDICATOR_LED_RED);
		_delay_ms(1000);
		INDICATOR_LED_PORT &= ~_BV(INDICATOR_LED_RED);
		//indicator blue
		_delay_ms(1000);
		INDICATOR_LED_PORT |= _BV(INDICATOR_LED_BLUE);
		_delay_ms(1000);
		INDICATOR_LED_PORT &= ~_BV(INDICATOR_LED_BLUE);
		//indicator green
		_delay_ms(1000);
		INDICATOR_LED_PORT |= _BV(INDICATOR_LED_GREEN);
		_delay_ms(1000);
		INDICATOR_LED_PORT &= ~_BV(INDICATOR_LED_GREEN);
	}

	return 0;
}
