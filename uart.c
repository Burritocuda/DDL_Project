/*
 * uart.c
 *
 *  Created on: April 17, 2018
 *      Author: Keifer Bowen, Jake Perez
 *
 *  This code has been very slightly modified from lab 3, but has
 *  mostly remained the same. This code configures our UART for
 *  transmission of 115200 bps. It also allows us to define
 *  how we send bytes and n-bytes of data to a terminal.
 *
 */


// other information
// MSP432401R pins used
// p3.2 = UCA2 RX
// p3.3 = UCA2 TX

#include "msp.h"
#include <stdint.h>
//#include "circbuf.h"
#include <uart.h>

extern volatile uint32_t cardDetect;
volatile uint8_t * data_array[6];
volatile uint8_t index=0;
uint8_t data;

// make some macros for configuring our BAUD rate
// #define CR (0x80) // for 115200
// #define br0 (0x01) // for 115200
// #define br1 (0x00) // for 115200

// 9600 BR with Aclk defines
#define br0 (0x03)
#define br1 (0x00)

// this function allows us to configure our BAUD rate to 115,200 bps or 9600 bps on port 2
void UART_Config_UCA2() {
    UCA2CTLW0 |= UCSWRST;   //begin initialization process for BAUD Rate 9600
    P3SEL0 |= BIT2 | BIT3;  // configure pins 3.2 and 3.3 to primary output mode, UART UCAB
    P3SEL1 &= ~(BIT2 | BIT3);
    //UCA2CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK; // for 115200 BR
    UCA2CTLW0 |= EUSCI_A_CTLW0_SSEL__ACLK; // Aux clk 32.768khz for 9600 BR
    UCA2CTLW0 &= ~EUSCI_A_CTLW0_PEN;   // parity disabled
    UCA2CTLW0 &= ~EUSCI_A_CTLW0_MODE0; // set to uart mode
    UCA2CTLW0 &= ~EUSCI_A_CTLW0_MODE1;
    UCA2CTLW0 &= ~EUSCI_A_CTLW0_MSB;       //lsb first
    UCA2CTLW0 &= ~EUSCI_A_CTLW0_SEVENBIT;  //8 bit data character length
    UCA2CTLW0 &= ~EUSCI_A_CTLW0_SPB;      // one stop bit one start bit is default
    // this is where we are with the UART config
    //UCA2MCTLW = 0xB5A1; // this is as bleow but for 115200 BR
    UCA2MCTLW = 0x0092; // this includes BRS and OS16 for 9600 BR
    UCA2BR0 = br0;     // set baud rate
    UCA2BR1 = br1;
    UCA2CTLW0 &= ~UCSWRST; // initialize eUSCI
    // configure interrupts for RX and TX
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;
    // enable the stuff that makes stuff happen when other stuff happens
    __NVIC_EnableIRQ(EUSCIA2_IRQn);
}

// this function allows us to configure our BAUD rate for 9600 bps, for port 0
void UART_Config_UCA0(void)
{
    UCA0CTLW0 |= UCSWRST;   //begin initialization process for BAUD Rate 9600
    P1SEL0 |= BIT2 | BIT3;  // configure pins 1.2 and 1.3 to primary output mode, UART UCAB
    P1SEL1 &= ~(BIT2 | BIT3);
    //UCA2CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK; // for 115200 BR
    UCA0CTLW0 |= EUSCI_A_CTLW0_SSEL__ACLK; // Aux clk 32.768khz for 9600 BR
    UCA0CTLW0 &= ~EUSCI_A_CTLW0_PEN;   // parity disabled
    UCA0CTLW0 &= ~EUSCI_A_CTLW0_MODE0; // set to uart mode
    UCA0CTLW0 &= ~EUSCI_A_CTLW0_MODE1;
    UCA0CTLW0 &= ~EUSCI_A_CTLW0_MSB;       //lsb first
    UCA0CTLW0 &= ~EUSCI_A_CTLW0_SEVENBIT;  //8 bit data character length
    UCA0CTLW0 &= ~EUSCI_A_CTLW0_SPB;      // one stop bit one start bit is default
    // this is where we are with the UART config
    //UCA2MCTLW = 0xB5A1; // this is as bleow but for 115200 BR
    UCA0MCTLW = 0x0092; // this includes BRS and OS16 for 9600 BR
    UCA0BR0 = br0;     // set baud rate
    UCA0BR1 = br1;
    UCA0CTLW0 &= ~UCSWRST; // initialize eUSCI
    // configure interrupts for RX and TX
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;
    // enable the stuff that makes stuff happen when other stuff happens
    __NVIC_EnableIRQ(EUSCIA0_IRQn);
}

// This function allows us to send n chars in a string when
// provided an array of chars and its associated length
void UART_send_n(uint8_t * data_array, uint32_t length) {
    //needs to send data if the transmission buffer is empty
    uint8_t i = 0;
    for(i = 0 ; i < length ; i++)
    {
        if(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)
        {
            UART_send_byte(data_array[i]);
        }
        else
            // otherwise dont increment past the char we didn't transmit
            i--;
    }
}
// This function sends a single byte of data out of the UART channel
void UART_send_byte(uint8_t data)
{
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    //send data if flag is ready
    EUSCI_A0->TXBUF = data;
}

void EUSCIA2_IRQHandler(void)
{
    if (EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG)
    {
        // if we receive info on UART, clear the IRQ flag and process the interrupt
        EUSCI_A2->IFG &= ~EUSCI_A_IFG_RXIFG;
        data = EUSCI_A2->RXBUF; // receive data from rx buffer
        data_array[index] = data;
        index++;
       UART_send_byte(data);  // send out data to confirm its valid with realterm
        cardDetect = 1;
    }
}



