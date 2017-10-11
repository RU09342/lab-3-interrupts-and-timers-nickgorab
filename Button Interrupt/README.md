# Button Blink
This program blinks an LED with the push of a button using an interrupt instead of polling. 

## Code Architecture
### Dependencies 
The `buttonBlink.c` code depends on two separate files. The first file is the generic MSP430  header (`msp430.h`), and a config file (`config.h`) which assigns the correct pins for each board. For more information about these, visit their respective `README.h` files.

```c
#include <msp430.h>
#include <config.h>
```
This is the section of the code which is used to include the header files.
### Functions 

#### Button Initialization

In order to make the code easier to digest, a void function was used to initialize the button.
```c
void buttonInit(void)
```
Inside of this function the button pin is declared an input, and the resistor is enabled and assigned to the pull up position. Additionally, the interrupt on the button pin is enabled, cleared, and set to falling edge. This function is then called inside of the main function and reduces the amount of possibly confusing code. The lines of code used to initialize the code are found below. 
```c 
B1_DIR |= ~BUTTON1;
B1_REN |= BUTTON1; 
B1_OUT |= BUTTON1; 
B1_IE  |= BUTTON1; 
B1_IES |= ~BUTTON1;
B1_IFG &= ~BUTTON1;
```




#### Main

In the main file there is no use of interrupts, so the Watch Dog timer needed to be disabled using the line 
```c
WDTCTL = WDTPW+WDTHOLD;
```
With the FR6989, FR2311, and FR5994 needing high impedance mode to be disabled, the line 
```c
HIGHZ;
```
is used to disable it. This is a macro defined in the config.h header file, and it will disable the high impedance mode for the specified board. For the other boards, it just opeartaes as a no-op. 

This project serves as a basic introduction to interrupts, so once the LED is set as an output using the line:
```c
L1_DIR |= LED1;
```
The program will just wait for an interrupt to occur. In order to save power while waiting for the interrupt, low-power mode is entered using the line found below.
```c
_BIS_SR(LPM0_bits + GIE);
```
This disables the CPU until an interrupt happens. The `GIE` bit at the end enables global interrupts. While there are other Low-Power modes that use less power, this one will cause the least amount of changes that need to be made in the code, preventing the timer source from changing.

#### Button ISR
When the button interrupt occurs, the ISR is entered. Inside of the button ISR, the LED is toggled in the same fashion as the previous labs, by XORing the LED bit. The button ISR is found below.
```c
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {

    L1_OUT ^= LED1;     // Toggles the LED on Pin 1.0 via XOR
    B1_IFG &= ~BUTTON1;    // Pin 1.1 Flag cleared
}
```
