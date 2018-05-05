/********************************************************************
 File Description:

 Contains code for reading ADC control inputs

********************************************************************/

#include <avr/io.h>

//Core definitions
#include "hardware.h"
#include "error_codes.h"

/**************************************/
/*! Sets up the ADC peripheral on the chip
 *
 * \retval #SUCCESS
 */
void adc_init()
{
    // AREF = AVcc ie. select Vcc as reference voltage by setting REFS1:0 = 01
    ADMUX = (1<<REFS0);

    // ADC Enable by setting ADEN
    //and prescaler of 128 by setting ADP2:0 to 1
    // ie. 16000000/128 = 125000
    ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

/**************************************/
/*! Reads an ADC channel on the chip
 *
 * \param ch Channel to read from
 * \retval value of ADC channel
 */
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
