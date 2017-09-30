/*
 * buttonInterrupt.c
 *
 *   Created on:  September 30, 2017
 *  Last Edited:  September 30, 2017
 *       Author:  Nick Gorab
 *        Board:  FR5994
 */

 #include "msp430.h"

void main(void) {

  WDTCTL = WDTPW | WDTHOLD;   // Disables the Watchdog timer
  PM5CTL0 &= ~LOCKLPM5;       // Disables high-impedance mode

  P1DIR |= BIT0;    // Sets Pin 1.0 as an output
  P5DIR |= ~BIT5;   // Sets Pin 5.5 as an input
  P5REN |= BIT5;    // Enables the resistor on Pin 5.5
  P5OUT |= BIT5;    // Enables the pull-up resistor for Pin 5.5

  P5IE  |= BIT5;   // Enables the interrupt on Pin 5.5
  P5IES |= ~BIT5;  // Rising edge detection
  P5IFG &= ~BIT5;  // Pin 5.5 Flag cleared

  _BIS_SR(LPM0_bits + GIE);   // Enables Low Power Mode 0 and Global Interrupt
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {

    P1OUT ^= BIT0;     // Toggles the LED on Pin 1.0 via XOR
    P5IFG &= ~BIT5;    // Pin 5.5 Flag cleared
}
