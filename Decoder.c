/*
 * Decoder.c
 *
 *  Created on: Apr 24, 2018
 *      Author: Jake
 */

#include "msp.h"
#include <stdint.h>
#include <stdio.h>

extern volatile uint8_t * data_array[16];
uint8_t data_1[16] = {0x02,0x31,0x44,0x30,0x30,0x41,0x41,0x42,0x30,0x31,0x43,0x31,0x42,0x0D,0x0A,0x03};

void decoder(void)
{
    uint8_t i = 0;
    uint8_t j;
    uint8_t holder;

   // __disable_irq();
    for(i = 0; i < 15; i++)
    {
       // holder=data_array[i];
        if(data_array[i] == data_1[i])
        {
            //P1->OUT ^= BIT0;
            //int k;
            //for(k = 0; k < 10000; k++);
        }
    }

    //reset values to zero
    for(j = 0; j < 15; j++)
    {
        data_array[j] = 0x00;
    }

    // __enable_irq();
 return;
}
