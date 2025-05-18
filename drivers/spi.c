#include <msp430.h>
#include "drivers/spi.h"

void spi_init()
{

    UCA0CTL1 |= UCSWRST;                  // Put USCI in reset
    UCA0CTL1 |= UCSSEL_2;                 // Select SMCLK as the clock source

    // Select transmittor mode, mode 0 (3-pin SPI), synchronous mode, MSB and inactive clock state to be high
    UCA0CTL0 = UCMST + UCMODE_0 + UCSYNC + UCMSB + UCCKPL;

    // Set timing as ~285 KHz, any faster and my setup doesn't work due to noise in my breadboard
    UCA0BR0 = 56;
    UCA0BR1 = 0;

    // GPIO init
    P1DIR |= CS + RST + DC;
    P1OUT |= CS;
    P1OUT |= RST;
    P1OUT |= DC;

    // SPI init
    P1SEL  |= MOSI + SCLK;
    P1SEL2 |= MOSI + SCLK;

    UCA0CTL1 &= ~UCSWRST;      // Release from reset, enabling SPI
}

void msp430_init()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    BCSCTL1 = CALBC1_16MHZ;      // Set freq to 16MHz
    DCOCTL = CALDCO_16MHZ;

    P2DIR |= BIT1;
    P2OUT &= ~BIT1;
}

void toggle_led()
{
    P2OUT ^= BIT1;
}
