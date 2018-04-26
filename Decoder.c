/*
 * Decoder.c
 *
 *  Created on: Apr 24, 2018
 *      Author: Jake
 */

#include "msp.h"
#include <stdint.h>
#include <stdio.h>

// 0x98,0xA0,0x98,0x9C,0x87,0xE0
extern volatile uint8_t * data_array;
uint8_t data_1[6] = {0x98,0xA0,0x98,0x9C,0x87,0xE0};
uint8_t i=0;
uint8_t j;
uint8_t holder;
void decoder(void)
{
    __disable_irq();
    for(i = 0; i < 6; i++)
    {
       // holder=data_array[i];
        if(data_array[i] == data_1[i])
        {
            P1->OUT ^= BIT0;
        }
    }

    //reset values to zero
    for(j = 0; j < 6; j++)
    {
        data_array[j] = 0x00;
    }
    __enable_irq();
 return;
}
