// this is the nmain file for use with

#include "msp.h"
#include <stdint.h>
#include "uart.h"
#include "GPIO.h"
#include "timer.h"
#include "Decoder.h"

extern volatile uint8_t * data_array;
extern volatile uint8_t index;
volatile uint32_t cardDetect = 0;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	UART_Config_UCA0(); // configure UART
	UART_Config_UCA2(); // configure UART
	GPIO_Config();      // configure GPIO PINS
	__enable_irq();     // global enable interrupts
	uint32_t i;

	// call configuration code here

	while(1)
	{
	  if(index>=5){
	      index=0;
	      decoder(data_array);
	    }
	    // toggle RED led
	    if (cardDetect)
	    {
	        P1->OUT ^= BIT0;
	        for(i = 0; i < 10000; i++); // delay loop
	        cardDetect = 0;
	    }
	}
}
