/********************************************************************
 File Description:

 definitions for the adc innterfaces

********************************************************************/

#ifndef ADC_H_
#define ADC_H_

/* FUNCTION DECLARATIONS *******************************************/

void adc_init();
uint16_t adc_read(uint8_t ch);
int adc_pos(int val);

#endif /* ADC_H_ */
