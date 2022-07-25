/********************************************************************
 File Description:

 Contains code for generation of output tones.

********************************************************************/

#include <math.h>
#include <avr/io.h>
#include <stdio.h>

#include "tone_generator.h"

#include "hardware.h"
#include "error_codes.h"

#include "uart.h"
#include "MCP4725.h"
#include "adc.h"
#include "wave.h"

volatile unsigned int advance;  //!< amount to advance though the wave table on each step

uint8_t toneEnabled;    //!< bool to store the current state of the generator
uint8_t toneZeroed;     //!< bool to store if the tone has been set to zero or not
uint16_t counter=0;     //!< current position in the wave table
uint16_t sinvol[512];

static const unsigned long tones[] = {550,600,650,700,750,800,850,900,950,1000,1050};
static const unsigned long volumes[] = {10,17,29,49,84,143,244,415,706,1203,2048};

#define TONE_FREQ 800UL //tone frequency in Hz
#define TONE_VOLUME 143
#define ADC_SAMPLE_RATE 15620UL //update rate of ADC in Hz

/*******************************************************************/
/*! Sets up the Tone generator in the default off state
 *
 * \return #SUCCESS
 *******************************************************************/
int ToneInit()
{
    toneEnabled=0;
    toneZeroed=1;

    SetTone(5);
    SetVolume(5);

    return SUCCESS;
}

/*******************************************************************/
/*! Calculates the next DAC voltage and sends to the I2C interface
 *
 * \return #INF_TONE_GENERATOR_HALTED if stopped otherwise #SUCCESS
 *******************************************************************/
int ToneCalculateNext()
{

    uint16_t Vout=0;

    //get output voltage
    Vout = sinvol[counter];
    counter = (counter + advance);

    if(counter>511 && (toneEnabled==0))
    {
        if(toneZeroed)
        {
            return(INF_TONE_GENERATOR_HALTED);
        }
        else
        {
            i2c_DAC_send_value(2048);
            toneZeroed = 1;
            return(SUCCESS);
        }
    }

    //send value to DAC
    i2c_DAC_send_value(Vout);

    counter &= 511;
    return(SUCCESS);
}

/*******************************************************************/
/*! Starts the tone generator
 *
 * \return #INF_TONE_GENERATOR_ALREADY_ACTIVE if already running otherwise #SUCCESS
 *******************************************************************/
int ToneStart()
{
    if(toneEnabled==1)
        return(INF_TONE_GENERATOR_ALREADY_ACTIVE);
    toneEnabled=1;
    toneZeroed=0;
    counter=0;
    ToneCalculateNext();
    return SUCCESS;
}

/*******************************************************************/
/*! Stops the tone generator
 *
 * \return #SUCCESS
 *******************************************************************/
int ToneStop()
{
    toneEnabled=0;
    return SUCCESS;
}

/*******************************************************************/
/*! Gets the tone status
 *
 * \return 1 if tone is running otherwise 0
 *******************************************************************/
int ToneEnabled()
{
    return toneEnabled;
}

/*******************************************************************/
/*! Sets the volume of the output tone
 *
 * \param volumeid position of the volume selector
 * \return #SUCCESS
 *******************************************************************/
int SetVolume(uint8_t volumeid)
{
    unsigned int volumeMod = ((2048UL*10UL)/volumes[volumeid]);

    int i=0;
    for(i=0;i<512;i++)
    {
        int32_t midVolt = WaveGetPoint(i);
        midVolt-= 2048;
        int32_t waveVolt = (midVolt*10)/volumeMod;
        waveVolt+=2048;
        sinvol[i] = waveVolt;

    }
    return SUCCESS;
}

/*******************************************************************/
/*! Sets the frequency of the output tone
 *
 * \param volumeid position of the tone selector
 * \return #SUCCESS
 *******************************************************************/
int SetTone(uint8_t toneid)
{
    uint32_t number = (tones[toneid] * 512UL);
    advance = (number/ADC_SAMPLE_RATE);

    return SUCCESS;
}
