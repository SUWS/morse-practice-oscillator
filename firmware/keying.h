/********************************************************************
 File Description:

 definitions for the I2C interface

********************************************************************/

#include <stdint.h>

#ifndef KEYING_H_
#define KEYING_H_

#define KEYER_ACTION_NONE 0
#define KEYER_ACTION_DOT 1
#define KEYER_ACTION_DASH 2

/* FUNCTION DECLARATIONS *******************************************/

int KeyingInit();
int KeyingProcess();

int KeyingSetRate(uint8_t rateid);
int KeyingSetWeight(uint8_t weightid);

#endif /* KEYING_H_ */
