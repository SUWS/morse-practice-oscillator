/********************************************************************
 File Description:

 definitions for the debug uart interface

********************************************************************/

#ifndef ERROR_CODES_H_
#define ERROR_CODES_H_

/* GENERAL CODES ***************************************************/
#define SUCCESS 0       //<! success (no error)


#define ERR_I2C_IN_USE 100  //<! I2C interface in use

#define INF_TONE_GENERATOR_HALTED 200   //!< tone generator is halted no next value
#define INF_TONE_GENERATOR_ALREADY_ACTIVE 201 //!< tone generator already active

#endif /* ERROR_CODES_H_ */
