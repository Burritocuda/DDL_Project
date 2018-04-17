/*
 * timer.c
 *
 *  Created on: Sep 11, 2017
 *      Author: Thomas Flynn
 *              Keifer Bowen
 */
#include "msp.h"

#define STRVR *(uint32_t *)0xE000E014
#define STCVR *(uint32_t *)0xE000E018
#define STCSR *(uint32_t *)0xE000E010

extern volatile uint8_t warmup;
extern volatile uint8_t take_out;
extern volatile uint16_t conv_flag;

// function definition for timer A configuration
void timer_a0_config(){
    TIMER_A0->R = 0;                             // Reset count to zero when we configure timer
    TIMER_A0->CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID_3 | TIMER_A_CTL_IE;   // SMCLK, Up mode, this bitmask sets upmode
    TIMER_A0->CCR[0] = 46875;                   // Value to count to
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;   // TACCR0 interrupt enabled
    TIMER_A0->EX0 = TIMER_A_EX0_TAIDEX_3;      // set clock divider to divide an additional X2 for X16 total - expansion reg

     //Enable Interrupts in the NVIC
    NVIC_EnableIRQ(TA0_0_IRQn);
    __NVIC_SetPriority(TA0_0_IRQn, 1);
}

// function definition for the timer interrupt
void TA0_0_IRQHandler() {
    __disable_irq();
    if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG) {
        TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG);
        // toggle blue LED during warmup Phase
        if (!warmup) {
            // toggle BLue LED
            P2->OUT &= ~(0x01);
            P2->OUT &= ~(0x02);
            P2->OUT ^= 0x04;
        }

        // blinking red LED when it needs to be
        // taken out
        if(take_out) {
            P2->OUT ^= 0x01;
            P2->OUT &= ~(0x02);
            P2->OUT &= ~(0x04);
        }
    }
    __enable_irq();
}

void systic_config() {
    STRVR |= 0x00FFFFFF; //program in max val to this register
    STCVR = 0; //clear this reg by writing any value to it
    STCSR |= 0x05; //config stcsr register for the required operation, run
}
