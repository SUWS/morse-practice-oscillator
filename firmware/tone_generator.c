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

volatile unsigned int advance;

uint8_t toneEnabled;
uint16_t counter=0;

static const unsigned long tones[] = {550,600,650,700,750,800,850,900,950,1000,1050};
static const unsigned long volumes[] = {0,0,200,300,400,512,768,1024,1200,1500,2048};

#define TONE_FREQ 800UL //tone frequency in Hz
#define TONE_VOLUME 512
#define ADC_SAMPLE_RATE 25000UL //update rate of ADC in Hz

int ToneInit()
{
    toneEnabled=0;

    SetTone(5);
    SetVolume(5);

    return 0;
}

int ToneCalculateNext()
{

    uint16_t Vout=0;

    //calculate output voltage
    Vout = 2048 + (((sintab2[counter])-2048)/4);
    counter = (counter + advance);

    if(counter>511 && (toneEnabled==0))
    {
        counter = 0;
        return(INF_TONE_GENERATOR_HALTED);
    }

    //send value to DAC
    i2c_DAC_send_value(Vout);

    counter &= 511;
    return(SUCCESS);
}

int ToneStart()
{
    if(toneEnabled==1)
        return(INF_TONE_GENERATOR_ALREADY_ACTIVE);
    toneEnabled=1;
    counter=0;
    ToneCalculateNext();
    return SUCCESS;
}

int ToneTest()
{

    while(1)
    {
        if(((KEY_IN_PORT_PIN >> KEY_STRAIGHT)&0x01)==1)
        {
            toneEnabled=0;
            while(((KEY_IN_PORT_PIN >> KEY_STRAIGHT)&0x01)==1);
            ToneStart();
            int adcval = adc_read(ADC_TONE);
            uint8_t toneid = adc_pos(adcval);
            SetTone(toneid);

            adcval = adc_read(ADC_VOLUME);
            uint8_t volumeid = adc_pos(adcval);
            SetVolume(volumeid);

            counter=0;

            char tmp[64];
    		sprintf(tmp,"NewTone :%d (%lu)\r\n",toneid,tones[toneid]);
    		UartPutString(tmp);
            sprintf(tmp,"NewVolume :%d (%lu)\r\n",volumeid,volumes[volumeid]);
    		UartPutString(tmp);

        }
    }
    return SUCCESS;
}

int SetVolume(unsigned int volumeid)
{
    unsigned int volumeMod = ((2048UL*10UL)/volumes[volumeid]);

    int i=0;
    for(i=0;i<512;i++)
    {
        int32_t midVolt = sintab2[i];
        midVolt-= 2048;
        int32_t waveVolt = (midVolt*10)/volumeMod;
        waveVolt+=2048;
        //toneTable[i] = waveVolt;

    }
    return SUCCESS;
}

int SetTone(unsigned int toneid)
{
    uint32_t number = (tones[toneid] * 512UL);
    advance = (number/ADC_SAMPLE_RATE);

    return SUCCESS;
}
