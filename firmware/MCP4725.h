/********************************************************************
 File Description:

 definitions for the I2C interface

********************************************************************/

#ifndef MCP4725_H_
#define MCP4725_H_

/* FUNCTION DECLARATIONS *******************************************/

int i2c_DAC_send_value(uint16_t DAC_Value);
//extern uint16_t sintab2[512];

#endif /* MCP4725_H_ */
