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

void adc_init()// [1]
{
    // AREF = AVcc ie. select Vcc as reference voltage by setting REFS1:0 = 01
    ADMUX = (1<<REFS0);
 
    // ADC Enable by setting ADEN
    //and prescaler of 128 by setting ADP2:0 to 1
    // ie. 16000000/128 = 125000
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

// adc_read() to read weight and rate analogue signal
uint16_t adc_read(uint8_t ch)// [2]
{
  // ch - channel select can enter the channel ie which ADC to use
  // we have ADC0-7 but we are only using ADC 1 and 0.
  // So we can only do adc_read(0) for reading from ADC0 pin for example.

  // Select the corresponding channel 0~1
  // ANDing with ’1′ will always keep the value
  // of ‘ch’ between 0 and 1
  ch &= 0b00000001;  // AND operation with 1
  ADMUX = (ADMUX & 0x00)|ch; // clears the 4 bits in MUX3..0 before before ORing it with ch
  // see table 21-3 on pdf 218/308 of datasheet for ATMEGA168 for ADMUX channel selection table

  // start single convertion
  // write ’1′ to ADSC
  ADCSRA |= (1<<ADSC);
 
  // wait for conversion to complete
  // ADSC becomes ’0′ again
  // till then, run loop continuously
  while(ADCSRA & (1<<ADSC));
 
  return (ADC);// spit out the ADC value of that channel
}

// void InitPWM()
// {
// 	// Using timer 0 
// 	// belows setting to enable the follwing
// 		// no prescaling on MCU clock (want highest freqz) (pg 90/308 , table 12-9)
// 		// mode: Fast PWM (pg 89/308 table 13-8)
// 		// Pwm output no inverting ( see table 12-9 on pdf 87/308)
// 	//for timer 0 wer using 
// 	// use timer counter control register 0  
// 	TCCR0A |=  _BV(WGM01) | _BV(WGM00) | _BV(COM0A1);// fast PWM, non - inverting outptu at Reg A of timer 0
// 	TCCR0B |= _BV(CS00);// no prescalar

// 	// set the 
// }
// // set duty cycle of ADC_KEY_OUT pin
// void setPWMOutput(uint8_t duty)
// {
// 	// Output comapre register 0 A contains the 8 bit value having value range 0-255
// 	OCR0A = duty;
// }

int main(int argc, char const *argv[])
{
	// Setting PC2(dot) & PC3(dash) as digital inputs
	DDRC &= ~_BV(PC2);
	DDRC &= ~_BV(PC3);
	for (;;)
	{
		/* code */
		uint8_t duty_cycle = adc_read(0);// read a value between 0-255
		uint8_t rate = adc_read(1)// read adc value for channel 1 
								  //for setting the speed of the pwm at KEY_OUT pin
								  // read a value between 0-255

		
		if(!(PINC & _BV(PC2)) // if we read a 0 from dot pin , we 
		{
			PORTC |= _BV(4);
			_delay_ms(100);// offset to prevent it from going fast
			_delay_ms(duty_cycle/rate);// remain high for this much delay time (0-255ms)
			// dots- detay of 1 time unit
			PORTC &= ~_BV(4);

		}	
		else if(!(PINC & _BV(PC3)))// if we read a 0 from dash pin ,  
			{
				PORTC |= _BV(4);
			_delay_ms(100);// offset to prevent it from going fast
			_delay_ms(3*duty_cycle/rate);// remain high for this much delay time (0-255ms)
			// dots- detay of 1 time unit
			PORTC &= ~_BV(4);

			}
		//_delay_ms(rate); // 0-255 ms

		/// DELAY OF 1 TIME UNIT BEWTEEEN DOTS & /OR DASHES OF A CHARCTER
		_delay_ms(duty_cycle/rate);
		// scale down all the delay() functions by rate 

	}


	return 0;
}
