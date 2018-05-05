/********************************************************************
 File Description:

 Contains code for using the debug uart interface

********************************************************************/

#include <avr/io.h>
#include <string.h>

//Core definitions
#include "hardware.h"
#include "error_codes.h"

#define USART_BAUDRATE 9600	//!< baud rate for the debug interface
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)	//!< value to use for baud rate prescaler


/*******************************************************************/
/*! Initialise the serial port and prepare ring buffer
 *
 * \return #SUCCESS as cannot fail
 *******************************************************************/
int UartInit()
{
	// Set baud rate
	UBRR0L = BAUD_PRESCALE;// Load lower 8-bits into the low byte of the UBRR register
	UBRR0H = (BAUD_PRESCALE >> 8);
	// Enable receiver and transmitter and receive complete interrupt
	UCSR0B = ((1<<TXEN0)|(1<<RXEN0));

	return SUCCESS;
}

/*******************************************************************/
/*! Sends a string to the debug uart
 *
 * \return errors encoutered
 *******************************************************************/
int UartPutString(char* str)
{
	uint16_t length = strlen(str);

	int i=0;
	for(i=0;i<length;i++)
	{
	  // Wait until last byte has been transmitted
	  while((UCSR0A &(1<<UDRE0)) == 0);

	  // Transmit data
	  UDR0 = str[i];
	}

	return SUCCESS;
}
