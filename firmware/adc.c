/********************************************************************
 File Description:

 Contains code for reading ADC control inputs

********************************************************************/

#include "adc.h"

#include <avr/io.h>

//Core definitions
#include "hardware.h"
#include "error_codes.h"

#include "tone_generator.h"
#include "keying.h"

Dials_t dials[] = {
    {ADC_TONE,5,SetTone},
    {ADC_VOLUME,5,SetVolume},
    {ADC_RATE,5,KeyingSetRate},
    {ADC_WEIGHT,5,KeyingSetWeight},
    {0,0,0},
};

uint8_t dialid;

/**************************************/
/*! Sets up the ADC peripheral on the chip
 *
 * \retval #SUCCESS
 */
void ADCInit()
{
    dialid=0;
    // AREF = AVcc ie. select Vcc as reference voltage by setting REFS1:0 = 01
    ADMUX = (1<<REFS0);

    // ADC Enable by setting ADEN
    //and prescaler of 128 by setting ADP2:0 to 1
    // ie. 16000000/128 = 125000
    ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

    //start reading of first dial
    ADCStartRead(dials[dialid].id);
}

/**************************************/
/*! Process to read settings from the ADC chanels
 *
 * Reads each channel in turn and triggers calbacks if the value has updated
 *
 * \retval #SUCCESS
 */
int ADCProcess()
{
    //if last conversion has finished
    if(!(ADCSRA & _BV(ADSC)))
    {
        //run callback for previous
        int dialval = adc_pos(ADC);
        if (dialval!=dials[dialid].lastValue)
        {
            dials[dialid].lastValue=dialval;
            dials[dialid].callback(dialval);
        }

        //get next dial
        dialid++;
        if(dials[dialid].callback==0)
        {
            dialid=0;
        }

        //start next dial
        ADCStartRead(dials[dialid].id);
    }

    return(SUCCESS);
}

/**************************************/
/*! Starts an ADC reading for a given channel
 *
 * \param ch Channel to read from
 * \retval #SUCCESS
 */
int ADCStartRead(uint8_t ch)// [2]
{
	ADMUX = ch;// represents PA2
	// start conversion
	ADCSRA |= _BV(ADSC);
    return(SUCCESS);
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
