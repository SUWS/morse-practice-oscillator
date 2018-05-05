/********************************************************************
 File Description:

 Contains main process for the morse sounder

********************************************************************/

#include <avr/io.h>

//Core definitions
#include "hardware.h"

//core libraies
#include "uart.h"

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

#define I2C_FREQ 400000

int i2c_setup()
{
	TWSR &= ~( _BV(TWPS1) | _BV(TWPS0) );
	TWBR = ((F_CPU / I2C_FREQ) - 16) / (2 * 1);
	TWCR = _BV(TWINT) + _BV(TWEA)+_BV(TWEN);
	TWAR = 0;
}

uint16_t sintab2[512] =
{
	2048, 2073, 2098, 2123, 2148, 2174, 2199, 2224,
	2249, 2274, 2299, 2324, 2349, 2373, 2398, 2423,
	2448, 2472, 2497, 2521, 2546, 2570, 2594, 2618,
	2643, 2667, 2690, 2714, 2738, 2762, 2785, 2808,
	2832, 2855, 2878, 2901, 2924, 2946, 2969, 2991,
	3013, 3036, 3057, 3079, 3101, 3122, 3144, 3165,
	3186, 3207, 3227, 3248, 3268, 3288, 3308, 3328,
	3347, 3367, 3386, 3405, 3423, 3442, 3460, 3478,
	3496, 3514, 3531, 3548, 3565, 3582, 3599, 3615,
	3631, 3647, 3663, 3678, 3693, 3708, 3722, 3737,
	3751, 3765, 3778, 3792, 3805, 3817, 3830, 3842,
	3854, 3866, 3877, 3888, 3899, 3910, 3920, 3930,
	3940, 3950, 3959, 3968, 3976, 3985, 3993, 4000,
	4008, 4015, 4022, 4028, 4035, 4041, 4046, 4052,
	4057, 4061, 4066, 4070, 4074, 4077, 4081, 4084,
	4086, 4088, 4090, 4092, 4094, 4095, 4095, 4095,
	4095, 4095, 4095, 4095, 4094, 4092, 4090, 4088,
	4086, 4084, 4081, 4077, 4074, 4070, 4066, 4061,
	4057, 4052, 4046, 4041, 4035, 4028, 4022, 4015,
	4008, 4000, 3993, 3985, 3976, 3968, 3959, 3950,
	3940, 3930, 3920, 3910, 3899, 3888, 3877, 3866,
	3854, 3842, 3830, 3817, 3805, 3792, 3778, 3765,
	3751, 3737, 3722, 3708, 3693, 3678, 3663, 3647,
	3631, 3615, 3599, 3582, 3565, 3548, 3531, 3514,
	3496, 3478, 3460, 3442, 3423, 3405, 3386, 3367,
	3347, 3328, 3308, 3288, 3268, 3248, 3227, 3207,
	3186, 3165, 3144, 3122, 3101, 3079, 3057, 3036,
	3013, 2991, 2969, 2946, 2924, 2901, 2878, 2855,
	2832, 2808, 2785, 2762, 2738, 2714, 2690, 2667,
	2643, 2618, 2594, 2570, 2546, 2521, 2497, 2472,
	2448, 2423, 2398, 2373, 2349, 2324, 2299, 2274,
	2249, 2224, 2199, 2174, 2148, 2123, 2098, 2073,
	2048, 2023, 1998, 1973, 1948, 1922, 1897, 1872,
	1847, 1822, 1797, 1772, 1747, 1723, 1698, 1673,
	1648, 1624, 1599, 1575, 1550, 1526, 1502, 1478,
	1453, 1429, 1406, 1382, 1358, 1334, 1311, 1288,
	1264, 1241, 1218, 1195, 1172, 1150, 1127, 1105,
	1083, 1060, 1039, 1017,  995,  974,  952,  931,
	910,  889,  869,  848,  828,  808,  788,  768,
	749,  729,  710,  691,  673,  654,  636,  618,
	600,  582,  565,  548,  531,  514,  497,  481,
	465,  449,  433,  418,  403,  388,  374,  359,
	345,  331,  318,  304,  291,  279,  266,  254,
	242,  230,  219,  208,  197,  186,  176,  166,
	156,  146,  137,  128,  120,  111,  103,   96,
	88,   81,   74,   68,   61,   55,   50,   44,
	39,   35,   30,   26,   22,   19,   15,   12,
	10,    8,    6,    4,    2,    1,    1,    0,
	0,    0,    1,    1,    2,    4,    6,    8,
	10,   12,   15,   19,   22,   26,   30,   35,
	39,   44,   50,   55,   61,   68,   74,   81,
	88,   96,  103,  111,  120,  128,  137,  146,
	156,  166,  176,  186,  197,  208,  219,  230,
	242,  254,  266,  279,  291,  304,  318,  331,
	345,  359,  374,  388,  403,  418,  433,  449,
	465,  481,  497,  514,  531,  548,  565,  582,
	600,  618,  636,  654,  673,  691,  710,  729,
	749,  768,  788,  808,  828,  848,  869,  889,
	910,  931,  952,  974,  995, 1017, 1039, 1060,
	1083, 1105, 1127, 1150, 1172, 1195, 1218, 1241,
	1264, 1288, 1311, 1334, 1358, 1382, 1406, 1429,
	1453, 1478, 1502, 1526, 1550, 1575, 1599, 1624,
	1648, 1673, 1698, 1723, 1747, 1772, 1797, 1822,
	1847, 1872, 1897, 1922, 1948, 1973, 1998, 2023
};

int i2c_DAC_send_value(uint16_t DAC_Value)
{
		//send a start command
		TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		while ((TWCR & (1<<TWINT)) == 0);
		
		//write address of DAC
		TWDR = 0xC0;
		TWCR = (1<<TWINT)|(1<<TWEN);
		while ((TWCR & (1<<TWINT)) == 0);
		
		//check for ack
		if ((TWSR & 0xF8) == 0x18)
		{
			KEY_LED_PORT |= _BV(LED_DOT);
			KEY_LED_PORT |= _BV(LED_DASH);
		}
		else
		{
			INDICATOR_LED_PORT |= _BV(INDICATOR_LED_RED);
		}
		
		//generate DAC data
		uint8_t DACmsg[2];
		DACmsg[0] = (DAC_Value>>8)&0x0F;
		DACmsg[1] = DAC_Value&0xFF;
		
		//send data
		TWDR = DACmsg[0];
		TWCR = (1<<TWINT)|(1<<TWEN);
		while ((TWCR & (1<<TWINT)) == 0);
		TWDR = DACmsg[1];
		TWCR = (1<<TWINT)|(1<<TWEN);
		while ((TWCR & (1<<TWINT)) == 0);
		
		//send a stop command
		TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	
}


int main(int argc, char const *argv[])
{
	adc_init();
    UartInit();

	// Setting PC2(dot) & PC3(dash) as digital inputs
	KEY_IN_PORT_DDR &= ~_BV(LED_DOT);
	KEY_IN_PORT_DDR &= ~_BV(LED_DASH);

	// set LEDs as OUTPUTS
	KEY_LED_PORT_DDR |= _BV(LED_DOT) + _BV(LED_DASH);
	INDICATOR_LED_DDR |= _BV(INDICATOR_LED_RED) + _BV(INDICATOR_LED_BLUE) + _BV(INDICATOR_LED_GREEN);
	
	/*
	 * I2C TEST
	 */
	
	//I2C test
	i2c_setup();
	
	int lookup=0;
	while(1)
	{
		i2c_DAC_send_value(sintab2[lookup]);
		lookup = (lookup + 5) & 511;
	}
	
	_delay_ms(5000);
	
	/*
	 * END I2C TEST
	 */

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
