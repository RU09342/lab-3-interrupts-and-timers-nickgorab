/*
 * timerABlink.c
 *
 *   Created on:  October 1, 2017
 *  Last Edited:  October 11, 2017
 *       Author:  Nick Gorab
 *        Board:  F2311
 */

 #include "msp430.h"
 #include "config.h"

unsigned int counter = 0, number = 0;

int converter(number){      
  TB0CCR0 = (64000/number);   // Function that converts from Hz
}


void main(void) {

  WDTCTL = WDTPW | WDTHOLD;   // Disables the Watchdog Timer
  HIGHZ;

  converter(4);
  L1_DIR |= LED1;    // Sets LED1 as an output
  L2_DIR |= LED2;

  TB0CCTL0 = CCIE;            // Enables the clock interrupt on overflow
  TB0CTL = TBSSEL_1 + MC_1;   // SMCLK, Up-Mode

  _BIS_SR(LPM0_bits + GIE);   // Enables Low Power Mode 0 and Global Interrupt
}


#pragma vector=TIMER0_B0_VECTOR

__interrupt void Timer_B (void) {

    L1_OUT ^= LED1;     // Toggles the LED on Pin 1.0 via XOR
    counter = counter + 1;

    if(counter == 4) {  // If statement that controls the rate of the second blinking LED
        L2_OUT ^= LED2;
        counter = 0;
    }
}
