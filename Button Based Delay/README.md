# Button Based Delay
This program blinks an LED at the length of a button press.

## Code Architecture
### Dependencies 
The `buttonBasedDelay.c` code depends on two separate files. The first file is the generic MSP430  header (`msp430.h`), and a config file (`config.h`) which assigns the correct pins for each board. For more information about these, visit their respective `README.h` files.

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
Inside of this function the button pin is declared an input, and the resistor is enabled and assigned to the pull up position. Additionally, the interrupt on the button pin is enabled, cleared, and set to falling edge. This function is then called inside of the main function and reduces the amount of possibly confusing code. This is done for both of the buttons on the board. The lines of code used to initialize the code are found below. 
```c 
B1_DIR |= ~BUTTON1;
B1_REN |=  BUTTON1;
B1_OUT |=  BUTTON1;
B1_IE  |=  BUTTON1;
B1_IES |= ~BUTTON1;
B1_IFG &= ~BUTTON1;
B2_DIR |= ~BUTTON2;
B2_REN |=  BUTTON2;
B2_OUT |=  BUTTON2;
B2_IE  |=  BUTTON2;
B2_IES |= ~BUTTON2;
B2_IFG &= ~BUTTON2;
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

The timer is then initalized using the code found below.
```c
TA0CCTL0 = CCIE;                
TA0CCR0 = 200;                  
TA0CTL = TASSEL_1 + MC_1 + ID_3;
```
This code enables the timer, sourcing SMCLK and operating in Up-Mode with a predivider of 8. The predivider will slow the clock down to the point where the maximum blink length can be 4 seconds. 

The program will then wait for an interrupt to occur. In order to save power while waiting for the interrupt, low-power mode is entered using the line found below.
```c
_BIS_SR(LPM0_bits + GIE);
```
This disables the CPU until an interrupt happens. The `GIE` bit at the end enables global interrupts. While there are other Low-Power modes that use less power, this one will cause the least amount of changes that need to be made in the code, preventing the timer source from changing.

#### Button 1 ISR
When the button interrupt occurs, the ISR is entered. Inside of the button ISR, the value of `CCR0`, which controls the rate of blinking, is set to zero. Then, the program keeps adding to the value of `CCR0` while the button is pressed down. Once the button interrupt flag is cleared, the ISR is exited. 
```c
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    __delay_cycles(500);          
    TA0CCR0 = 0;                  
    while((BUTTON1 & B1_IN) == 0){
        TA0CCR0 = TA0CCR0 + 1;    
    }
    B1_IFG &= ~BUTTON1;
}
```

#### Timer ISR
When the timer interrupt occurs, it simply flashes the LED. Note that the timer interrupt does not need to be reset. 
```c
#pragma vector=TIMER0_A0_VECTOR

__interrupt void Timer_A (void) {
    L1_OUT ^= LED1;     // Toggles the LED on Pin 1.0 via XOR
}
```
## Differences Between Boards
As you may have noticed, the MSP430F2311 has its own special folder for itself. This is beacuse the board does not have a Timer_A, so Timer_B needs to be used instead. This is a simple fix, just not something that I included in the `config.h` file. For example, instead of `TA0CCR0`, it changes to `TB0CCR0`.


## Extra Work
## Reset Button
When the second button is pressed, the Button 2 ISR is entered. This ISR just resets the value of the `CCR0` integer back to 200, which is 10 Hz. The code for the ISR can be found below. 
```c
#pragma vector=PORT2_VECTOR
__interrupt void Port2(void) {
    __delay_cycles(500);
    TA0CCR0 = 200;      
    B2_IFG &= ~BUTTON2; 
}
```