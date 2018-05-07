/********************************************************************
 File Description:

 definitions for the adc innterfaces

********************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

/* dials struct */

typedef struct {
    uint8_t id; //!< ADC channel of the dial ADC port
    uint8_t lastValue;  //!< last value of the dial
    int (*callback)(uint8_t); //!< callback to run on change
} Dials_t;

/* FUNCTION DECLARATIONS *******************************************/

void ADCInit();
int ADCProcess();
int ADCStartRead(uint8_t ch);
int adc_pos(int val);

#endif /* ADC_H_ */
