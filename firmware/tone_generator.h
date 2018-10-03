/********************************************************************
 File Description:

 definitions for the I2C interface

********************************************************************/

#include <stdint.h>

#ifndef TONE_GENERATOR_H_
#define TONE_GENERATOR_H_

/* FUNCTION DECLARATIONS *******************************************/

int ToneInit();
int ToneCalculateNext();
int ToneStart();
int ToneStop();
int ToneEnabled();
int SetTone(uint8_t toneid);
int SetVolume(uint8_t volumeid);

#endif /* TONE_GENERATOR_H_ */
