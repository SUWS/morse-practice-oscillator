/********************************************************************
 File Description:

 Contains code for using the i2c interface

********************************************************************/

#include "i2c.h"

#include <avr/io.h>

//Core definitions
#include "hardware.h"
#include "error_codes.h"

#define I2C_FREQ 400000

/*******************************************************************/
/*! Sets up the I2C peripheral
 *
 * \return #SUCCESS as cannot fail
 *******************************************************************/
int I2CInit()
{
	TWSR &= ~( _BV(TWPS1) | _BV(TWPS0) );
	TWBR = ((F_CPU / I2C_FREQ) - 16) / (2 * 1);
	TWCR = _BV(TWINT) + _BV(TWEA)+_BV(TWEN);
	TWAR = 0;
}
