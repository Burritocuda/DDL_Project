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
extern int status;
uint8_t enable=1;//lock enable disable parameter
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
        if(enable ==1){//checks lock ready

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
    }
    else if(u2==15){
        UART_send_n("user_2 accessed device\n", strlen("user_2 accessed device\n"));
        if(enable ==1){//checks lock ready

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
    }
    else if(m==15){
        UART_send_n("Master access granted\n", strlen("Master access granted\n"));
        UART_send_n("1. Lock Status\n2. Lock\n3. Unlock\n4. Lock Ready\n5. Enable Lock\n6. Disable Lock", strlen("1. Lock Status\n2. Lock\n3. Unlock\n4. Lock Ready\n5. Enable Lock\n6. Disable Lock"));
        if(1){
            if(status==1){
                UART_send_n("Open",strlen("Open"));
            }
            else if(status==0){
                UART_send_n("Closed",strlen("Closed"));
            }
        }
        else if(2){
            if(status==0){
                UART_send_n("Already Locked",strlen("Already Locked"));
            }
            else if(status==1){
                pwm_down();
                UART_send_n("unlocked",strlen("unlocked"));
            }
        }
        else if(3){
            if(status==1){
                          UART_send_n("Already Unlocked",strlen("Already Unlocked"));
                      }
                      else if(status==0){
                          pwm_up();
                          UART_send_n("locked",strlen("locked"));
                      }
        }
        else if(4){
            if(enable==1){
                UART_send_n("Enabled",strlen("Enabled"));
            }
            else if(enable==0){
                UART_send_n("Disabled",strlen("Disabled"));
            }
        }
        else if(5){
            if(enable==1){
                UART_send_n("Already enabled",strlen("Already enabled"));
            }
            else if(enable==0){
            enable=1;
            UART_send_n("enabled",strlen("enabled"));
            }
        }
        else if(6){
            if(enable==0){
                        UART_send_n("Already disabled",strlen("Already disabled"));
                    }
                    else if(enable==1){
                    enable=0;
                    UART_send_n("disabled",strlen("disabled"));
                    }
        }
    }
    else{
        UART_send_n("unknown user tried to access the device\n", strlen("unknown user tried to access the device\n"));
    }
    //reset values to zero
    for(j = 0; j < 15; j++)
    {
        data_array[j] = 0x00;
    }

    // __enable_irq();
 return;
}
