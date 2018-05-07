/********************************************************************
 File Description:

Manages the Iambic keying sequence

********************************************************************/

#include "keying.h"

#include <avr/io.h>

//Core definitions
#include "hardware.h"
#include "error_codes.h"

#include "tone_generator.h"

uint8_t keyerLastAction;
uint8_t keyerRate;
uint8_t keyerWeight;

static const unsigned long rates[] = {3000,2800,2600,2400,2200,2000,1800,1600,1400,1200,1000};

int KeyingInit()
{
    keyerLastAction=KEYER_ACTION_NONE;
    keyerRate=5;
    keyerWeight=5;

    //setup timer 1
    TCCR1A = 0x00;
    TCCR1B =_BV(CS12) + _BV(CS10);
    TCCR1C = 0x00;

    TCNT1 = 0x00;

    TIFR1 = _BV(OCF1A) + _BV(OCF1B);

}

int KeyingSetRate(int rateid)
{
    keyerRate=rateid;
    return(SUCCESS);
}

int KeyingSetWeight(int weightid)
{
    keyerWeight=weightid;
    return(SUCCESS);
}

int KeyingSetSymbol(int length)
{
    int16_t weightmod = (keyerWeight-5)*(rates[keyerRate]/10);
    OCR1A = (rates[keyerRate]*length)+weightmod;
    OCR1B = (rates[keyerRate]*(length+1));
    TCNT1 = 0;
    TIFR1 = _BV(OCF1A) + _BV(OCF1B);
}

int KeyingGetNextAction()
{
    if(keyerLastAction==KEYER_ACTION_DOT)
    {
        if(!(KEY_IN_PORT_PIN & _BV(KEY_DASH)))
        {
            keyerLastAction=KEYER_ACTION_DASH;
            KeyingSetSymbol(3);
            ToneStart();
        }
        else if(!(KEY_IN_PORT_PIN & _BV(KEY_DOT)))
        {
            keyerLastAction=KEYER_ACTION_DOT;
            KeyingSetSymbol(1);
            ToneStart();
        }
        else
        {
            keyerLastAction=KEYER_ACTION_NONE;
        }
    }
    else if(keyerLastAction==KEYER_ACTION_DASH)
        {
            if(!(KEY_IN_PORT_PIN & _BV(KEY_DOT)))
            {
                keyerLastAction=KEYER_ACTION_DOT;
                KeyingSetSymbol(1);
                ToneStart();
            }
            else if(!(KEY_IN_PORT_PIN & _BV(KEY_DASH)))
            {
                keyerLastAction=KEYER_ACTION_DASH;
                KeyingSetSymbol(3);
                ToneStart();
            }
            else
            {
                keyerLastAction=KEYER_ACTION_NONE;
            }
        }
    else
    {
        keyerLastAction=KEYER_ACTION_NONE;
    }
}

int KeyingProcess()
{
    switch(keyerLastAction)
    {
        case KEYER_ACTION_NONE:
            if(!(KEY_IN_PORT_PIN & _BV(KEY_DOT)))
            {
                KeyingSetSymbol(1);
                ToneStart();
                keyerLastAction=KEYER_ACTION_DOT;
            }
            if(!(KEY_IN_PORT_PIN & _BV(KEY_DASH)))
            {
                KeyingSetSymbol(1);
                ToneStart();
                keyerLastAction=KEYER_ACTION_DOT;
            }
            else
            {
                ToneStop();
            }
            break;
        case KEYER_ACTION_DOT:
        case KEYER_ACTION_DASH:
            if(TIFR1 & _BV(OCF1B))
            {
                //end of symbol
                KeyingGetNextAction();
            }
            else if(TIFR1 & _BV(OCF1A))
            {
                //endo of tone
                ToneStop();
            }
            break;
        default:
            INDICATOR_LED_PORT &= ~_BV(INDICATOR_LED_RED);
    }
}
