/*
 * buttonInterrupt.c
 *
 *   Created on:  September 30, 2017
 *  Last Edited:  October 11, 2017
 *       Author:  Nick Gorab
 *        Board:  ALL
 */

 #include "msp430.h"
 #include "config.h"

void buttonInit(void){
  B1_DIR |= ~BUTTON1;   // Sets Pin 1.1 as an input
  B1_REN |= BUTTON1;    // Enables the resistor on Pin 1.1
  B1_OUT |= BUTTON1;    // Enables the pull-up resistor for Pin 1.1
  B1_IE  |= BUTTON1;   // Enables the interrupt on Pin 1.1
  B1_IES |= ~BUTTON1;  // Rising edge detection
  B1_IFG &= ~BUTTON1;  // Pin 1.1 Flag cleared
}

void main(void) {

  WDTCTL = WDTPW | WDTHOLD;   // Disables the Watchdog Timer

  HIGHZ;
  buttonInit();
  L1_DIR |= LED1;    // Sets Pin 1.0 as an output
 

  _BIS_SR(LPM0_bits + GIE);   // Enables Low Power Mode 0 and Global Interrupt
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {

    L1_OUT ^= LED1;     // Toggles the LED on Pin 1.0 via XOR
    B1_IFG &= ~BUTTON1;    // Pin 1.1 Flag cleared
}
