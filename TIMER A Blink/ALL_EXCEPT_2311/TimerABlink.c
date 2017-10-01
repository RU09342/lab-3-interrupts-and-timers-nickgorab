/*
 * timerABlink.c
 *
 *   Created on:  October 1, 2017
 *  Last Edited:  October 1, 2017
 *       Author:  Nick Gorab
 *        Board:  F5529, FR5994, FR6989, G2553
 */

 #include "msp430.h"
 #include "config.h"

unsigned int counter = 0;

void main(void) {

  WDTCTL = WDTPW | WDTHOLD;   // Disables the Watchdog Timer
  HIGHZ;


  L1_DIR |= LED1;    // Sets LED1 as an output
  L2_DIR |= LED2;

  TA0CCTL0 = CCIE;
  TA0CCR0 = 8170;
  TA0CTL = TASSEL_1 + MC_1;

  _BIS_SR(LPM0_bits + GIE);   // Enables Low Power Mode 0 and Global Interrupt
}


#pragma vector=TIMER0_A0_VECTOR

__interrupt void Timer_A (void) {

    L1_OUT ^= LED1;     // Toggles the LED on Pin 1.0 via XOR
    counter = counter + 1;

    if(counter == 4) {
        L2_OUT ^= LED2;
        counter = 0;
    }
}
