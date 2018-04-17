/*
 * uart.h
 *
 *  Created on: April 17,2018
 *      Author: Keifer Bowen, Jake Perez
 *
 *  This file makes all the function declarations for our uart.h file.
 *  These functions are used extensively in this lab.
 */

#include "stdint.h"

#ifndef UART_H_
#define UART_H_

// configure our UART function declaration
void UART_Configure(void);

// set up function for sending a single byte of data, declaration
void UART_send_n(uint8_t * data_array, uint32_t length);

/* set up function for iterating over an array, calling the the
 * send single byte function until it sends the entire sequence
 * of bytes
 */
void UART_send_byte(uint8_t data);

#endif /* UART_H_ */
