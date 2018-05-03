/********************************************************************
 File Description:

 Contains pin definitions and other hardware specific options
 Hardware definitions are for r2.0 sounder hardware

********************************************************************/


#ifndef HARDWARE_H_
#define HARDWARE_H_

#define F_CPU 16000000

/* INDICATOR LED ***************************************************/

#define INDICATOR_LED_PORT PORTD  //!< port register for indicator LED
#define INDICATOR_LED_PIN PIND    //!< pin register for indicator LED
#define INDICATOR_LED_DDR DDRD    //!< data direction register for indicator led

#define INDICATOR_LED_RED PD2     //!< indicator LED red
#define INDICATOR_LED_BLUE PD3     //!< indicator LED blue
#define INDICATOR_LED_GREEN PD4     //!< indicator LED green

/* KEY INPUT *******************************************************/

#define KEY_IN_PORT PORTB         //!< port register for Key input
#define KEY_IN_PORT_PIN PINB      //!< pin register for key input
#define KEY_IN_PORT_DDR DDRB      //!< data direction register for key input

#define KEY_DOT PB0               //!< dot paddle connection also straight key input
#define KEY_STRAIGHT PB0          //!< straight key input
#define KEY_DASH PB1              //!< dash paddle connection

/* KEY INPUT LEDS **************************************************/

#define KEY_LED_PORT PORTD        //!< port register for key led indicators
#define KEY_LED_PORT_PIN PIND     //!< pin register for key led indicators
#define KEY_LED_PORT_DDR DDRD     //!< data direction register for key led indicators

#define LED_DOT PD5               //!< dot indicator LED
#define LED_DASH PD6              //!< dash indicator LED

/* SETTING INPUTS **************************************************/

#define ADC_SETTINGS_PORT PORTC   //!< port register for potentiometer input
#define ADC_SETTINGS_PORT_PIN PINC //!< pin register for potentiometer input
#define ADC_SETTINGS_PORT_DDR DDRC  //!< data direction register for potentiometer input

#define ADC_WEIGHT PC0            //!< weight setting input
#define ADC_RATE PC1              //!< rate setting input
#define ADC_VOLUME PC2            //!< weight setting input
#define ADC_TONE PC3              //!< rate setting input

#define KEY_MODE_PORT PORTD       //!< port register for key mode
#define KEY_MODE_PORT_PIN PIND    //!< pin register for key mode
#define KEY_MODE_PORT_DDR DDRD    //!< data direction register for key mode

#define KEY_MODE PD7              //!< key mode input

/* DEBUG UART INTERFACE ********************************************/

#define I2C_PORT PORTD            //!< port register for I2C
#define I2C_PIN PIND              //!< pin register for I2C
#define I2C_DDR DDRD              //!< data direction register for I2C

#define I2C_SDA PC4              //!< I2C SDA pin
#define I2C_SCL PC5              //!< I2C SCL pin

/* AMP DEBUG *******************************************************/

#define AMP_DEBUG_PORT PORTC      //!< port register for amplifier debug
#define AMP_DEBUG_PIN PINC        //!< pin register for amplifier debug
#define AMP_DEBUG_DDR DDRC        //!< data direction register for amplifier debug

#define AMP_DEBUG PC6             //!< amplifier debug input

/* DEBUG UART INTERFACE ********************************************/

#define DEBUG_UART_PORT PORTD     //!< port register for debug uart
#define DEBUG_UART_PORT_PIN PIND  //!< pin register for debug uart
#define DEBUG_UART_PORT_DDR DDRD  //!< data direction register for debug uart

#define DEBUG_TX PD1              //!< debug uart transmit pin
#define DEBUG_RX PD0              //!< debug uart receive pin

#endif /* HARDWARE_H_ */
