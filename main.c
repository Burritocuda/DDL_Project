// this is the nmain file for use with

#include "msp.h"
#include <stdint.h>
#include "uart.h"
#include "GPIO.h"
#include "timer.h"

volatile uint32_t cardDetect = 0;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	UART_Config();  // configure UART
	GPIO_Config();      // configure GPIO PINS
	__enable_irq();     // global enable interrupts
	uint32_t i;

	// call coniguration code here

	while(1)
	{
	    // toggle RED led
	    if (cardDetect)
	    {
	        P1->OUT ^= BIT0;
	        for(i = 0; i < 10000; i++); // delay loop
	        cardDetect = 0;
	    }


	}
}
