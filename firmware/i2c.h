/********************************************************************
 File Description:

 definitions for the I2C interface

********************************************************************/

#include <stdint.h>

#ifndef I2C_H_
#define I2C_H_

/* FUNCTION DECLARATIONS *******************************************/

int I2CInit();
int I2CSend(uint8_t* data,uint8_t len);

#endif /* I2C_H_ */
