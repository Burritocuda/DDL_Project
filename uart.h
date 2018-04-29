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
#include <stdio.h>
#ifndef UART_H_
#define UART_H_

// configure our UART function declaration, port 2
void UART_Config_UCA2(void);

// configure our UART function, port 0
void UART_Config_UCA0(void);

// set up function for sending a single byte of data, declaration
void UART_send_n(uint8_t * data_array, uint32_t length);

/* set up function for iterating over an array, calling the the
 * send single byte function until it sends the entire sequence
 * of bytes
 */
void UART_send_byte(uint8_t data);

void add_item_to_array(uint8_t store_data);
#endif /* UART_H_ */
