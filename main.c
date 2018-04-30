// this is the nmain file for use with

#include "msp.h"
#include <stdint.h>
#include "uart.h"
#include "GPIO.h"
#include "timer.h"
#include "Decoder.h"

extern volatile uint8_t index;
volatile uint32_t cardDetect = 0;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    CS->KEY = CS_KEY_VAL;                   // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_3;           // Set DCO to 12MHz (nominal, center of 8-16MHz range)
    CS->CTL1 = CS_CTL1_SELA_2 |             // Select ACLK = REFO
            CS_CTL1_SELS_3 |                // SMCLK = DCO
            CS_CTL1_SELM_3;                 // MCLK = DCO

    CS->KEY = 0;
	UART_Config_UCA0(); // configure UARTs
	UART_Config_UCA2(); // configure UART
	timer_a0_pwm_config();
	GPIO_Config();      // configure GPIO PINS
	__enable_irq();     // global enable interrupts
	//uint32_t i;

	// call configuration code here

	while(1)
	{
	   if(index >= 16)
	   {
	      cardDetect=1;
	      index = 0;
	      decoder();
	    }
	    // toggle RED led
//	    if (cardDetect)
//	    {
	       // P1->OUT ^= BIT0;
	      //  cardDetect = 0;

	    //}
	}
}
