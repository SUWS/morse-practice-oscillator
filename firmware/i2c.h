/********************************************************************
 File Description:

 definitions for the I2C interface

********************************************************************/

#include <stdint.h>

#ifndef I2C_H_
#define I2C_H_

/* I2C command */

typedef struct {
    volatile uint8_t status;
    uint8_t address;
    uint8_t length;
    uint8_t pos;
    uint8_t buffer[8];
} i2c_message_t;

#define I2C_STATE_IDLE 0
#define I2C_STATE_ACTIVE 1

#define I2C_MESSAGE_STATE_ACTIVE 0
#define I2C_MESSAGE_STATE_DONE 1

/* FUNCTION DECLARATIONS *******************************************/

int I2CInit();
int I2CSend(i2c_message_t *message);

#endif /* I2C_H_ */
