// this is the nmain file for use with

#include "msp.h"
#include <stdint.h>
#include "uart.h"



/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}
