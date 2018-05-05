/********************************************************************
 File Description:

 Contains code for using the i2c interface

********************************************************************/

#include "i2c.h"

#include <avr/io.h>
#include <avr/interrupt.h>

//Core definitions
#include "hardware.h"
#include "error_codes.h"

#define I2C_FREQ 800000

/*******************************************************************/
/*! Sets up the I2C peripheral
 *
 * \return #SUCCESS as cannot fail
 *******************************************************************/
int I2CInit()
{
	//disable prescaller and set TWBR to correct clock rate
	TWSR &= ~( _BV(TWPS1) | _BV(TWPS0) );
	TWBR = ((F_CPU / I2C_FREQ) - 16) / (2 * 1);

	//set I2C pins as inputs and turn on pull up resistors
	I2C_DDR &= ~(_BV(I2C_SDA) | _BV(I2C_SCL));
	I2C_PORT |= _BV(I2C_SDA) | _BV(I2C_SCL);

	TWCR = _BV(TWINT) + _BV(TWEA)+_BV(TWEN);

	//not a slave
	TWAR = 0;

	return SUCCESS;
}

int I2CSend(uint8_t* data,uint8_t len)
{
		//send a start command
		TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		while ((TWCR & (1<<TWINT)) == 0);

		//send data
		uint8_t i;
		for (i=0;i<len;i++)
		{
			TWDR = data[i];
			TWCR = (1<<TWINT)|(1<<TWEN);
			while ((TWCR & (1<<TWINT)) == 0);

			//TODO handle this nicer
			/*
			if ((TWSR & 0xF8) != 0x18)
			{
				//send a stop command
				TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
				return(1);
			}
			*/
		}

		//send a stop command
		TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
        return SUCCESS;
}

ISR(TWI_vect,ISR_BLOCK) {

}
