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

// function definition for timer A configuration

// changing this configuration for PWM for controlling a servo
void timer_a0_config()
{
    TIMER_A0->R = 0;                             // Reset count to zero when we configure timer
    TIMER_A0->CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID_3 | TIMER_A_CTL_IE;   // SMCLK, Up mode, this bitmask sets upmode
    TIMER_A0->CCR[0] = 46875;                   // Value to count to
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;   // TACCR0 interrupt enabled
    TIMER_A0->EX0 = TIMER_A_EX0_TAIDEX_3;      // set clock divider to divide an additional X2 for X16 total - expansion reg

     //Enable Interrupts in the NVIC
    NVIC_EnableIRQ(TA0_0_IRQn);
    //__NVIC_SetPriority(TA0_0_IRQn, 1);
}
void timer_a0_pwm_config()
{
    // configure pin 2.4 as the output for our PWM
    P2->SEL0 |= BIT6;  // set to primary function mode, for PWM
    P2->SEL1 &= ~BIT6;
    P2->DIR |= BIT6;
    //TIMER_A0->R = 0; // reset time count to configure
    // up mode, smclk, clock divider 1, timer A interupt enable
    TIMER_A0->CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID_1;
    // we have to use ccr1 and cctl1 becuase the PWM mode does not work on ccr0 and cctl0
    // according to the documentation
    TIMER_A0->CCR[0] = 8000;
    TIMER_A0->CCR[3]= 4000; // value we want to count too
    // toggle/reset mode (PWM), capture capare interrupt enable
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;
    //Enable Interrupts in the NVIC
    //NVIC_EnableIRQ(TA0_0_IRQn);
}

// function definition for the timer interrupt
void TA0_0_IRQHandler()
{
    __disable_irq();
    __enable_irq();
}

void systic_config()
{
    STRVR |= 0x00FFFFFF; //program in max val to this register
    STCVR = 0; //clear this reg by writing any value to it
    STCSR |= 0x05; //config stcsr register for the required operation, run
}
