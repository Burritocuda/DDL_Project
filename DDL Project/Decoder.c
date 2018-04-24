/*
 * Decoder.c
 *
 *  Created on: Apr 24, 2018
 *      Author: Jake
 */

#include "msp.h"
#include <stdint.h>
#include <stdio.h>


extern volatile uint8_t * data_array;



void decoder(volatile uint8_t * data_array){
 if(data_array[0]==0x98){
  if(data_array[1]==0xA0){
      if(data_array[2]==0x8D){
          if(data_array[3]==0x87){
              if(data_array[4]==0xe0){
                  P1->OUT ^= BIT0;
              }
          }
      }
  }
}
 data_array[0]=0;
 data_array[1]=0;
 data_array[2]=0;
 data_array[3]=0;
 data_array[4]=0;
 data_array[5]=0;
 return;
}
