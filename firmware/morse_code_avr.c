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

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

uint16_t uart_out_string(char* str)
{
	uint16_t length = strlen(str);
	
	int i=0;
	for(i=0;i<length;i++)
	{
	  // Wait until last byte has been transmitted
	  while((UCSR0A &(1<<UDRE0)) == 0);

	  // Transmit data
	  UDR0 = str[i];
	}
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



int main(int argc, char const *argv[])
{
	adc_init();
	// Set baud rate
	UBRR0L = BAUD_PRESCALE;// Load lower 8-bits into the low byte of the UBRR register
	UBRR0H = (BAUD_PRESCALE >> 8); 
	// Enable receiver and transmitter and receive complete interrupt 
	UCSR0B = ((1<<TXEN0)|(1<<RXEN0));
	
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
		
		char tmp[24];
		int adcval = adc_read(ADC_WEIGHT);
		sprintf(tmp,"Weight :%d (%d)\r\n",adc_pos(adcval),adcval);
		//sprintf(tmp,"Weight :%d\r\n",adc_read(ADC_WEIGHT));
		uart_out_string(tmp);
		
		adcval = adc_read(ADC_RATE);
		sprintf(tmp,"Rate :%d (%d)\r\n",adc_pos(adcval),adcval);
		//sprintf(tmp,"Rate :%d\r\n",adc_read(ADC_RATE));
		uart_out_string(tmp);
		
		adcval = adc_read(ADC_VOLUME);
		sprintf(tmp,"Volume :%d (%d)\r\n",adc_pos(adcval),adcval);
		//sprintf(tmp,"Rate :%d\r\n",adc_read(ADC_RATE));
		uart_out_string(tmp);
		
		adcval = adc_read(ADC_TONE);
		sprintf(tmp,"Tone :%d (%d)\r\n",adc_pos(adcval),adcval);
		//sprintf(tmp,"Rate :%d\r\n",adc_read(ADC_RATE));
		uart_out_string(tmp);
		
		adcval = adc_read(AMP_DEBUG);
		sprintf(tmp,"Amp Debug :%d (",adcval);
		//sprintf(tmp,"Rate :%d\r\n",adc_read(ADC_RATE));
		uart_out_string(tmp);
		if(adcval<128) uart_out_string("OUTL to VBAT)\r\n");
		else if(adcval<384) uart_out_string("OUTR to VBAT)\r\n");
		else if(adcval<640) uart_out_string("OUTL to GND)\r\n");
		else if(adcval<896) uart_out_string("OUTR to GND)\r\n");
		else uart_out_string("NO Fault)");
		
		int switchPos = (KEY_MODE_PORT_PIN >> KEY_MODE)&0x01;
		sprintf(tmp,"Switch :%d \r\n",switchPos);
		uart_out_string(tmp);
		
		int keyPos = (KEY_IN_PORT_PIN >> KEY_DOT)&0x01;
		sprintf(tmp,"Dot :%d \r\n",keyPos);
		uart_out_string(tmp);
		
		keyPos = (KEY_IN_PORT_PIN >> KEY_DASH)&0x01;
		sprintf(tmp,"Dash :%d \r\n",keyPos);
		uart_out_string(tmp);
		
		uart_out_string("#####\r\n");
		_delay_ms(1000);
		
		
	}

	return 0;
}
