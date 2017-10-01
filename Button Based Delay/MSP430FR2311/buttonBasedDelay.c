/*
 * buttonBasedDelay.c
 *
 *   Created on:  October 1, 2017
 *  Last Edited:  October 1, 2017
 *       Author:  Nick Gorab
 *        Board:  F5529, FR5994, FR6989, G2553
 */

 #include "msp430.h"
 #include "config.h"


void main(void) {

  WDTCTL = WDTPW | WDTHOLD;   // Disables the Watchdog Timer
  HIGHZ;


  L1_DIR |= LED1;    // Sets LED1 as an output
  L2_DIR |= LED2;

  B1_DIR |= ~BUTTON1;   // Sets Pin 1.1 as an input
  B1_REN |= BUTTON1;    // Enables the resistor on Pin 1.1
  B1_OUT |= BUTTON1;    // Enables the pull-up resistor for Pin 1.1

  B1_IE  |= BUTTON1;   // Enables the interrupt on Pin 1.1
  B1_IES |= ~BUTTON1;  // Rising edge detection
  B1_IFG &= ~BUTTON1;  // Pin 1.1 Flag cleared

  TB0CCTL0 = CCIE;
  TB0CCR0 = 32768;
  TB0CTL = TBSSEL_1 + MC_1;

  _BIS_SR(LPM0_bits + GIE);   // Enables Low Power Mode 0 and Global Interrupt
}


#pragma vector=TIMER0_B0_VECTOR

__interrupt void Timer_B (void) {

    L1_OUT ^= LED1;     // Toggles the LED on Pin 1.0 via XOR

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    __delay_cycles(500);

    TB0CCR0 = 0;

    while((BUTTON1 & B1_IN) == 0){
        TB0CCR0 = TB0CCR0 + 1;
    }

    B1_IFG &= ~BUTTON1;    // Pin 1.1 Flag cleared
}
