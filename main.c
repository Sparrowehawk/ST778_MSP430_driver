#include <msp430.h> 

#define SCLK BIT4
#define MOSI BIT2
// No MiSO in this driver
#define CS   BIT0
#define RST  BIT1
#define DC   BIT3

#include <stdint.h>
#include <stdbool.h>

/**
 * main.c
 */
void spi_init()
{

    UCA0CTL1 |= UCSWRST;                  // Put USCI in reset
    UCA0CTL1 |= UCSSEL_2;                 // Select SMCLK as the clock source

    UCA0CTL0 = UCMST + UCMODE_0 + UCSYNC + UCMSB; // Selects UCSI to be SPI mode

    // Sets timing for 100KHz as 1MHz (SMLK) / 10 = 100k
    UCA0BR0 = 10;
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

    BCSCTL1 = CALBC1_1MHZ;      // Set freq to 1MHz
    DCOCTL = CALDCO_1MHZ;

    P2DIR |= BIT1;
}

void st778_write_command(uint8_t cmdd)
{
    P1OUT &= ~CS;              // Select device (active low)
    P1OUT &= ~DC;              // D/C = 0 (command mode)
    spi_write_byte(cmd);      // Send 0xAE
    P1OUT |= CS;               // Deselect device
}

void st778_write_data(uint8_t data)
{
    P1OUT &= ~CS;              // Select device (active low)
    P1OUT |= DC;              // D/C = 0 (command mode)
    spi_write_byte(data);      // Send 0xAE
    P1OUT |= CS;               // Deselect device
}

void st778_write_data_16bit(uint16_t data)
{
    spi_write_data(data >> 8);
    spi_write_data(data & 0xFF);
}

void spi_write_byte(uint8_t data)
{
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = data;
}

void st778_init()
{
    // Tell the ST778 about the display and whatnot
}

void st778_set_addr()
{
    // Tell what LCD it is writing to
}

void st778_draw_pixel()
{
    // Takes in X, Y, and colour
    // Used to write text
    // Needs font.h with a ASCII with 96 rows 5 cols
    // EXAMPLES:
        // asci[0][0~4] => 0x00 0x00 0x00 0x00 0x00 => Space
        // asci[1][0~4] => 0x00 0x00 0x5f 0x00 0x00 => !
        // Each value represents a column, each column is 8 rows with the MSB from the top / LSB from bot
}

void st778_draw_char(uint16_t x, uint16_t y, uint16_t colour, unsigned char letter)
{
    // uses draw_pixel to draw a charecter
    // Draw entire screen in that colour
    // ex
    // int i;
    // for (i = 0; i < 5; i++){
    //    st778_draw_char(x + (i * 5), y, ascii[letter][i]);
    // }
}

void st778_clear_screen(uint16_t colour)
{

}

void st778_draw_string(uint16_t x, uint16_t y, uint16_t colour, char* m)
{
    // loop something like
    // for (i = 0; greetings[i] != '\0'; i++) {
    //      st778_draw_char(x, y, colour, m[i]
    // }
}
int main(void)
{
    msp430_init();
    spi_init();

    // Reset LCD
    P1OUT &= ~RST;
    __delay_cycles(100000);
    P1OUT |= RST;

    P2OUT ^= BIT1;
    // Send command
    P1OUT &= ~CS;              // Select device (active low)
    P1OUT &= ~DC;              // D/C = 0 (command mode)
    spi_write_byte(0xAE);      // Send 0xAE
    P1OUT |= CS;               // Deselect device
    P2OUT ^= BIT1;


    while (1);
}

