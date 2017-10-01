/*
 * timerABlink.c
 *
 *   Created on:  October 1, 2017
 *  Last Edited:  October 1, 2017
 *       Author:  Nick Gorab
 *        Board:  FR2311
 */

 #include "msp430.h"
 #include "config.h"

unsigned int counter = 0;

void main(void) {

  WDTCTL = WDTPW | WDTHOLD;   // Disables the Watchdog Timer
  HIGHZ;


  L1_DIR |= LED1;    // Sets LED1 as an output
  L2_DIR |= LED2;

  TB0CCTL0 = CCIE;
  TB0CCR0 = 8170;
  TB0CTL = TBSSEL_1 + MC_1;

  _BIS_SR(LPM0_bits + GIE);   // Enables Low Power Mode 0 and Global Interrupt
}


#pragma vector=TIMER0_B0_VECTOR

__interrupt void Timer_B (void) {

    L1_OUT ^= LED1;     // Toggles the LED on Pin 1.0 via XOR
    counter = counter + 1;

    if(counter == 4) {
        L2_OUT ^= LED2;
        counter = 0;
    }
}
