/********************************************************************
 File Description:

 Contains code for using the i2c interface

********************************************************************/

#include "i2c.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>		//I2C defintions

//Core definitions
#include "hardware.h"
#include "error_codes.h"

#include "tone_generator.h"

#define I2C_FREQ 800000

uint8_t i2cState;
volatile i2c_message_t* i2cMessage;

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

	TWCR = _BV(TWIE) + _BV(TWINT) + _BV(TWEA) + _BV(TWEN);

	//not a slave
	TWAR = 0;

	sei();

	return SUCCESS;
}

int I2CSend(i2c_message_t *message)
{
	if(i2cState!=I2C_STATE_IDLE)
	{
		INDICATOR_LED_PORT &= ~_BV(INDICATOR_LED_RED);
		return(ERR_I2C_IN_USE);
	}
	i2cMessage=message;
	i2cMessage->pos=0;
	i2cMessage->status=I2C_MESSAGE_STATE_ACTIVE;

	//send a start command
	TWCR = (1<<TWIE)|(1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWEA);
	i2cState=I2C_STATE_ACTIVE;

	return(SUCCESS);
}

ISR(TWI_vect,ISR_BLOCK) {
	if(i2cState==I2C_STATE_IDLE)
		return;
	switch(TW_STATUS) {
		case TW_START:
		case TW_REP_START:
			TWDR=i2cMessage->address;
			TWCR=_BV(TWIE) + _BV(TWINT) + _BV(TWEA)+_BV(TWEN);
			break;

		case TW_MT_SLA_ACK:
			TWDR=i2cMessage->buffer[i2cMessage->pos++];
			TWCR=_BV(TWIE) + _BV(TWINT) + _BV(TWEA)+_BV(TWEN);
			break;

		case TW_MT_DATA_ACK:
			if(i2cMessage->pos<i2cMessage->length)
			{
				TWDR = i2cMessage->buffer[i2cMessage->pos++];
				TWCR=_BV(TWIE) + _BV(TWINT) + _BV(TWEA)+_BV(TWEN);
			}
			else
			{
				i2cMessage->status=I2C_MESSAGE_STATE_DONE;
				i2cState=I2C_STATE_IDLE;

				//if no next tone then send a stop signal (repeted start sent otherwise)
				if(ToneCalculateNext())
				{
					TWCR = _BV(TWIE) + _BV(TWINT) + _BV(TWEA)+_BV(TWEN) + _BV(TWSTO);
				}
			}
			break;
	}

	return;
}
