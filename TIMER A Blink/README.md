# Timer_A Blink
This program blinks two LEDs with the use of a timer instead of software loop. 

## Code Architecture
### Dependencies 
The `timerABlink.c` code depends on two separate files. The first file is the generic MSP430  header (`msp430.h`), and a config file (`config.h`) which assigns the correct pins for each board. For more information about these, visit their respective `README.h` files.

```c
#include <msp430.h>
#include <config.h>
```
This is the section of the code which is used to include the header files.
### Functions 

#### Converter

In order to convert a given Hz value to something the code can use, this function is used. 
```c
int converter(number)
	```
Inside of this function the value 64000 is divided by the speficied value `number`, and the resulting value is set to the CCR0 value of Timer A.
```c 
TA0CCR0 = (64000/number);
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

The `converter` function is used to calaculate the correct CCR0 value, using the line of code found below.
```c
converter(4);
```
The LEDs are then initialized by setting them as an output. 

In order to use timers to toggle the LED, Timer_A needed to be initialized. This is done by using the code found below. 
```c
TA0CCTL0 = CCIE;
TA0CTL = TASSEL_1 + MC_1;
```
The first line enables the interrupts for the clock when it overflows, and the second line determines the behavior of the clock. In this instance, it sources SMCLK, and runs in Up-Mode. This means whenever it hits the specified CCR0 value, it triggers an interrupt. 

#### Timer ISR
When the timer interrupt occurs, the ISR is entered. Inside of the timer ISR, the LED is toggled in the same fashion as the previous labs, by XORing the LED bit. In order to blink the second LED, a variable `counter` is used to determine how fast it blinks. There is a for loop which checks to see if counter is equal to a specific value, blinks the LED, and then resets the coutner to zero. The timer ISR is found below.
```c
#pragma vector=TIMER0_A0_VECTOR

__interrupt void Timer_A (void) {

    L1_OUT ^= LED1;
    counter = counter + 1;

    if(counter == 4) {  
        L2_OUT ^= LED2;
        counter = 0;
    }
}
```

## Differences Between Boards
As you may have noticed, the MSP430F2311 has its own special folder for itself. This is beacuse the board does not have a Timer_A, so Timer_B needs to be used instead. This is a simple fix, just not something that I included in the `config.h` file. For example, instead of `TA0CCR0`, it changes to `TB0CCR0`.

## Extra Work
### Low-Power Timers
In order to save power while the timer is counting, low-power mode is entered using the line found below.
```c
_BIS_SR(LPM0_bits + GIE);
```
This disables the CPU until an interrupt happens. The `GIE` bit at the end enables global interrupts. While there are other Low-Power modes that use less power, this one will cause the least amount of changes that need to be made in the code, preventing the timer source from changing. 
## Bugs
Setting the `number` variable equal to zero will result in the code trying to divide by zero, which will result in the code not working correctly.