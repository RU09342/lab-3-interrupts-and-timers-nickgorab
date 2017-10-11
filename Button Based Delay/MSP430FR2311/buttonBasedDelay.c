/*
 * buttonBasedDelay.c
 *
 *   Created on:  October 1, 2017
 *  Last Edited:  October 11, 2017
 *       Author:  Nick Gorab
 *        Board:  F2311
 */

 #include "msp430.h"
 #include "config.h"

void buttonInit(void) {
  B1_DIR |= ~BUTTON1;    // Sets Button 1 as an input
  B1_REN |=  BUTTON1;    // Enables the resistor on Button 1
  B1_OUT |=  BUTTON1;    // Enables the pull-up resistor for Button 1
  B1_IE  |=  BUTTON1;    // Enables the interrupt on Pin Button 1
  B1_IES |= ~BUTTON1;    // Rising edge detection
  B1_IFG &= ~BUTTON1;    // Button 1 Flag cleared

  B2_DIR |= ~BUTTON2;    // Sets Button 2 as an input
  B2_REN |=  BUTTON2;    // Enables the resistor on Button 2
  B2_OUT |=  BUTTON2;    // Enables the pull-up resistor for Button 2
  B2_IE  |=  BUTTON2;    // Enables the interrupt on Pin Button 2
  B2_IES |= ~BUTTON2;    // Rising edge detection
  B2_IFG &= ~BUTTON2;    // Button 2 Flag cleared
}


void main(void) {

  WDTCTL = WDTPW | WDTHOLD;   // Disables the Watchdog Timer
  HIGHZ;


  L1_DIR |= LED1;    // Sets LED1 as an output

  buttonInit();

  TB0CCTL0 = CCIE;                   // Interrupt Enable
  TB0CCR0 = 200;                     // 10 Hz
  TB0CTL = TBSSEL_1 + MC_1 + ID_3;   //SMCLK, Up-Mode, Predivider of 8

  _BIS_SR(LPM0_bits + GIE);          // Enables Low Power Mode 0 and Global Interrupt
}


#pragma vector=TIMER0_B0_VECTOR

__interrupt void Timer_B (void) {

    L1_OUT ^= LED1;     // Toggles the LED on Pin 1.0 via XOR

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    __delay_cycles(500);            // Debouncing

    TB0CCR0 = 0;                    // Resets the CCR0 value

    while((BUTTON1 & B1_IN) == 0){  // Sees if the button is pressed
        TB0CCR0 = TB0CCR0 + 1;      // Increases the CCR0 value depending on how long button is pressed
    }

    B1_IFG &= ~BUTTON1;    // Pin 1.1 Flag cleared
}

#pragma vector=PORT2_VECTOR
__interrupt void Port2(void) {
    __delay_cycles(500);   // Debouncing

    TB0CCR0 = 200;         // Resets the CCR0 value to 10 Hz

    B2_IFG &= ~BUTTON2;    // Pin 1.1 Flag cleared
}