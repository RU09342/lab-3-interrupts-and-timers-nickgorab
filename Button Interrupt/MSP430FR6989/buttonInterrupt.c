/*
 * buttonInterrupt.c
 *
 *   Created on:  September 30, 2017
 *  Last Edited:  September 30, 2017
 *       Author:  Nick Gorab
 *        Board:  FR6989
 */

 #include "msp430.h"

void main(void) {

  WDTCTL = WDTPW | WDTHOLD;   // Disables the Watchdog Timer
  PM5CTL0 &= ~LOCKLPM5;       // Disables high-impedance mode

  P1DIR |= BIT0;    // Sets Pin 1.0 as an output
  P1DIR |= ~BIT1;   // Sets Pin 1.3 as an input
  P1REN |= BIT1;    // Enables the resistor on Pin 1.3
  P1OUT |= BIT1;    // Enables the pull-up resistor for Pin 1.3

  P1IE  |= BIT1;   // Enables the interrupt on Pin 1.3
  P1IES |= ~BIT1;  // Rising edge detection
  P1IFG &= ~BIT1;  // Pin 1.3 Flag cleared

  _BIS_SR(LPM0_bits + GIE);   // Enables Low Power Mode 0 and Global Interrupt
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {

    P1OUT ^= BIT0;     // Toggles the LED on Pin 1.0 via XOR
    P1IFG &= ~BIT1;    // Pin 1.3 Flag cleared
}
