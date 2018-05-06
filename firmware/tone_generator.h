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
int SetTone(unsigned int );
int SetVolume(unsigned int volumeid);

#endif /* TONE_GENERATOR_H_ */
