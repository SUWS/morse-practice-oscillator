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


int main(int argc, char const *argv[])
{
	adc_init();
    UartInit();
    I2CInit();
    LEDInit();

	// Setting PC2(dot) & PC3(dash) as digital inputs
	KEY_IN_PORT_DDR &= ~_BV(LED_DOT);
	KEY_IN_PORT_DDR &= ~_BV(LED_DASH);

	/*
	 * I2C TEST
	 */
    ToneTest();

	_delay_ms(5000);

	/*
	 * END I2C TEST
	 */

	while(1)
	{
		//dot indicator LED
		//if((KEY_IN_PORT_PIN & _BV(KEY_DOT)))
		_delay_ms(1000);
		LEDDotOn();
		//else
		_delay_ms(1000);
		LEDDotOff();

		//dash indicator LED
		//if((KEY_IN_PORT_PIN & _BV(KEY_DASH)))
		_delay_ms(1000);
		LEDDashOn();
		_delay_ms(1000);
		//else
		LEDDashOff();


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
		UartPutString(tmp);

		adcval = adc_read(ADC_RATE);
		sprintf(tmp,"Rate :%d (%d)\r\n",adc_pos(adcval),adcval);
		//sprintf(tmp,"Rate :%d\r\n",adc_read(ADC_RATE));
		UartPutString(tmp);

		adcval = adc_read(ADC_VOLUME);
		sprintf(tmp,"Volume :%d (%d)\r\n",adc_pos(adcval),adcval);
		//sprintf(tmp,"Rate :%d\r\n",adc_read(ADC_RATE));
		UartPutString(tmp);

		adcval = adc_read(ADC_TONE);
		sprintf(tmp,"Tone :%d (%d)\r\n",adc_pos(adcval),adcval);
		//sprintf(tmp,"Rate :%d\r\n",adc_read(ADC_RATE));
		UartPutString(tmp);

		adcval = adc_read(AMP_DEBUG);
		sprintf(tmp,"Amp Debug :%d (",adcval);
		//sprintf(tmp,"Rate :%d\r\n",adc_read(ADC_RATE));
		UartPutString(tmp);
		if(adcval<128) UartPutString("OUTL to VBAT)\r\n");
		else if(adcval<384) UartPutString("OUTR to VBAT)\r\n");
		else if(adcval<640) UartPutString("OUTL to GND)\r\n");
		else if(adcval<896) UartPutString("OUTR to GND)\r\n");
		else UartPutString("NO Fault)");

		int switchPos = (KEY_MODE_PORT_PIN >> KEY_MODE)&0x01;
		sprintf(tmp,"Switch :%d \r\n",switchPos);
		UartPutString(tmp);

		int keyPos = (KEY_IN_PORT_PIN >> KEY_DOT)&0x01;
		sprintf(tmp,"Dot :%d \r\n",keyPos);
		UartPutString(tmp);

		keyPos = (KEY_IN_PORT_PIN >> KEY_DASH)&0x01;
		sprintf(tmp,"Dash :%d \r\n",keyPos);
		UartPutString(tmp);

		UartPutString("#####\r\n");
		_delay_ms(1000);


	}

	return 0;
}
