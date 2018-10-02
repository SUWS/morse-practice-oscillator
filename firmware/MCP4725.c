/********************************************************************
 File Description:

 Contains code for interfacing with the MCP4725 DAC chip

********************************************************************/

#include <avr/io.h>

#include "MCP4725.h"

//Core definitions
#include "hardware.h"
#include "error_codes.h"

#include "i2c.h"

volatile i2c_message_t DACmsg;

int i2c_DAC_send_value(uint16_t DAC_Value)
{
		DACmsg.address=0xC0;
		DACmsg.buffer[0] = (DAC_Value>>8)&0x0F;
		DACmsg.buffer[1] = DAC_Value&0xFF;
		DACmsg.length=2;
		I2CSend(&DACmsg);

		return SUCCESS;
}
