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
extern uint8_t response;
extern uint8_t status;
uint8_t enable=1;//lock enable disable parameter
uint8_t toggle = 1;
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
        response ='0';
        UART_send_n("1. Lock Status\n2. Lock\n3. Unlock\n4. Lock Ready\n5. Enable Lock\n6. Disable Lock\n7. Exit\n", strlen("1. Lock Status\n2. Lock\n3. Unlock\n4. Lock Ready\n5. Enable Lock\n6. Disable Lock\n7. Exit\n"));

        while(response != '1' || '2' || '3' || '4' || '5' || '6' || '7' || '8')
        {
        if(response=='1')
        {
            if(status==1)
            {
                UART_send_n("Unlocked\n",strlen("Unlocked\n"));
            }
            else if(status==0)
            {
                UART_send_n("Locked\n",strlen("Locked\n"));
            }
            response ='8';
            //return;
        }
        else if(response=='2'){
            if(status==0){
                UART_send_n("Already Locked\n",strlen("Already Locked\n"));
            }
            else if(status==1){
                pwm_down();
                toggle++;
                toggle = toggle%2;
                UART_send_n("unlocked\n",strlen("unlocked\n"));
            }
            response ='8';
            //return;
        }
        else if(response=='3'){
            if(status==1){
                          UART_send_n("Already Unlocked\n",strlen("Already Unlocked\n"));
                      }
                      else if(status==0){
                          pwm_up();
                          toggle++;
                          toggle = toggle%2;
                          UART_send_n("locked\n",strlen("locked\n"));
                      }
            response ='8';
           // return;
        }
        else if(response=='4'){
            if(enable==1){
                UART_send_n("Enabled\n",strlen("Enabled\n"));
            }
            else if(enable==0){
                UART_send_n("Disabled\n",strlen("Disabled\n"));
            }
            response ='8';
            //return;
        }
        else if(response=='5'){
            if(enable==1){
                UART_send_n("Already enabled\n",strlen("Already enabled\n"));
            }
            else if(enable==0){
            enable=1;
            UART_send_n("enabled\n",strlen("enabled\n"));
            }
            response ='8';
           // return;
        }
        else if(response=='6'){
            if(enable==0){
                        UART_send_n("Already disabled\n",strlen("Already disabled\n"));
                    }
                    else if(enable==1){
                    enable=0;
                    UART_send_n("disabled\n",strlen("disabled\n"));
                    }
            response ='8';
           // return;
        }
        else if(response=='7'){
            UART_send_n("exiting\n",strlen("exiting\n"));
            response = '0';
            return;
        }

        else if(response == '0');// do nothing
        else
        {
            UART_send_n("Unknown response\n", strlen("Unknown response\n"));
            response ='8';
        }
        if(response == '8')
        {
            UART_send_n("1. Lock Status\n2. Lock\n3. Unlock\n4. Lock Ready\n5. Enable Lock\n6. Disable Lock\n7. Exit\n", strlen("1. Lock Status\n2. Lock\n3. Unlock\n4. Lock Ready\n5. Enable Lock\n6. Disable Lock\n7. Exit\n"));
            response = '0';
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
