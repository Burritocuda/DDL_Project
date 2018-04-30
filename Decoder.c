/*
 * Decoder.c
 *
 *  Created on: Apr 24, 2018
 *      Author: Jake
 */

#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include "string.h"

extern volatile uint8_t * data_array[16];
uint8_t toggle = 0;
uint8_t user_1[16] = {0x02,0x31,0x44,0x30,0x30,0x41,0x41,0x42,0x30,0x31,0x43,0x31,0x42,0x0D,0x0A,0x03};
uint8_t user_2[16] = {0x02,0x31,0x44,0x30,0x30,0x41,0x41,0x43,0x35,0x32,0x32,0x35,0x30,0x0D,0x0A,0x03};
uint8_t master[16] = {0x02,0x31,0x44,0x30,0x30,0x41,0x41,0x43,0x46,0x45,0x44,0x39,0x35,0x0D,0x0A,0x03};

void decoder(void)
{
    uint8_t i = 0;
    uint8_t j;
    uint8_t holder;
    int u1=0;
    int u2=0;
    int m=0;

   // __disable_irq();
    for(i = 0; i < 15; i++)
    {
       // holder=data_array[i];
        if(data_array[i] == user_1[i])
        {
            u1++;
        }
        if(data_array[i] == user_2[i]){
            u2++;
        }
        if(data_array[i] == master[i]){
            m++;
        }
    }
    if(u1==15){
        UART_send_n("user_1 accessed device\n", strlen("user_1 accessed device\n"));
        if(toggle == 0)
                  {
                      pwm_up();
                  }

                  //for(i = 0; i < 100000; i++); // delay loop
                  if(toggle == 1)
                  {
                      pwm_down();
                  }

                  toggle++;
                  toggle = toggle%2;
    }
    else if(u2==15){
        UART_send_n("user_2 accessed device\n", strlen("user_2 accessed device\n"));
        if(toggle == 0)
                  {
                      pwm_up();

                  }

                  //for(i = 0; i < 100000; i++); // delay loop
                  if(toggle == 1)
                  {
                      pwm_down();
                  }

                  toggle++;
                  toggle = toggle%2;
    }
    else if(m==15){
        UART_send_n("Master access granted\n", strlen("Master access granted\n"));

    }
    else{
        UART_send_n("unknown user accessed the device\n", strlen("unknown user accessed the device\n"));
    }
    //reset values to zero
    for(j = 0; j < 15; j++)
    {
        data_array[j] = 0x00;
    }

    // __enable_irq();
 return;
}
