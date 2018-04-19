/*
 * GPIO.c
 *
 *  Created on: Apr 19, 2018
 *      Author: Keifer Bowen, Jake Perez
 *
 *
 */
#include "msp.h"



// this code configures all general purpose input output pins for use
// with our program. SUch as UART and PWM
void GPIO_Config()
{
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0; // set general purpose I/O mode
    P1->DIR |= BIT0; // set direction to out
    P1->OUT &= ~BIT0; // make sure LED is off Initially
}
