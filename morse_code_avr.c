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

// define all the pins 
#define ADC_WEIGHT 0//PC0
#define ADC_RATE 1//PC1
#define ADC_KEY_OUT PD3
#define ADC_KEY_MODE PD7
#define LED_DOT PD5
#define LED_DASH PD6
#define KEY_DOT PB0
#define KEY_DASH PB1
#include <avr/io.h>
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
	ADMUX = n;// represents PA2
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
	DDRC &= ~_BV(LED_DOT);
	DDRC &= ~_BV(LED_DASH);

	// setting ADC_KEY_OUT (PD3) as digital out

	DDRC |= _BV(ADC_KEY_OUT);
	for (;;)
	{
		/* code */
		uint8_t duty_cycle = adc_read(ADC_WEIGHT);// read a value between 0-255
		uint8_t rate = adc_read(ADC_RATE)// read adc value for channel 1 
								  //for setting the speed of the pwm at KEY_OUT pin
								  // read a value between 0-255

		
		if(!(PINC & _BV(LED_DOT)) // if we read a 0 (active low) from dot pin , we 
		{
			PORTC |= _BV(ADC_KEY_OUT);
			_delay_ms(100);// offset to prevent it from going fast
			_delay_ms(duty_cycle/rate);// remain high for this much delay time (0-255ms)
			// dots- length of 1 time unit
			PORTC &= ~_BV(ADC_KEY_OUT);

		}	
		else if(!(PINC & _BV(PC3)))// if we read a 0 (active low input) from dash pin ,  
			{
				PORTC |= _BV(ADC_KEY_OUT);
			_delay_ms(100);// offset to prevent it from going fast
			_delay_ms(3*duty_cycle/rate);// remain high for this much delay time (0-255ms)
			// dash- length of 3 time unit
			PORTC &= ~_BV(ADC_KEY_OUT);

			}
		//_delay_ms(rate); // 0-255 ms

		/// DELAY OF 1 TIME UNIT BEWTEEEN DOTS & /OR DASHES OF A CHARCTER
		_delay_ms(duty_cycle/rate);
		// scale down all the delay() function by rate to adjust the speed 

	}


	return 0;
}
