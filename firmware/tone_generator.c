/********************************************************************
 File Description:

 Contains code for generation of output tones.

********************************************************************/

#include <math.h>
#include <avr/io.h>
#include "MCP4725.h"

uint8_t toneEnabled;

#define TONE_FREQ 800UL //tone frequency in Hz
#define TONE_VOLUME 512
#define ADC_RATE 25000UL //update rate of ADC in Hz

int ToneInit()
{
    toneEnabled=0;

    return 0;
}

int ToneTest()
{
    int counter=0;
    while(1)
    {
        uint16_t Vout=0;

        //next step calculate
        unsigned int advance = ((TONE_FREQ * 512UL)/ADC_RATE);

        //calculate output voltage
        Vout = 1024 + (((sintab2[counter])-1024)/4);
        counter = (counter + advance) & 511;

        i2c_DAC_send_value(Vout);
    }
}
