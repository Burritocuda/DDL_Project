
#include "msp.h"
#include "timer.h"

#define STRVR *(uint32_t *)0xE000E014
#define STCVR *(uint32_t *)0xE000E018
#define STCSR *(uint32_t *)0xE000E010

// function definition for timer A configuration

// changing this configuration for PWM for controlling a servo
uint8_t status=1;


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
    //TIMER_A0->R = 0; // reset time count to configure
    // up mode, smclk, clock divider 1, timer A interupt enable
    TIMER_A0->CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID__4  ;
    // we have to use ccr1 and cctl1 becuase the PWM mode does not work on ccr0 and cctl0
    // according to the documentation
    TIMER_A0->CCR[0] = 60000; // 50HZ
    TIMER_A0->CCR[3] = 5000; // 20% duty cycle
    // toggle/reset mode (PWM), capture capare interrupt enable
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;
    //Enable Interrupts in the NVIC
    //NVIC_EnableIRQ(TA0_0_IRQn);
}
// locked
void pwm_up()
{
    __disable_irq();
    //timer_a0_pwm_config(3200);
    // 6400
    TIMER_A0->CCR[3] = 5000;
    uint32_t i;
    for(i = 0; i < 100000; i++); // delay loop
    //timer_a0_pwm_config(100);
    __enable_irq();
    status=1; //to tell if the lock is open or not, 1=open
}

// unlocked
void pwm_down()
{
    __disable_irq();
    //timer_a0_pwm_config(900);
    // 1800
    TIMER_A0->CCR[3] = 1800;
    uint32_t i;
    for(i = 0; i < 100000; i++); // delay loop
    //timer_a0_pwm_config(100);
    __enable_irq();
    status=0;//closed
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
