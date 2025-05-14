#include <msp430.h> 
#include "drivers/spi.h"

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

void spi_write_byte(uint8_t data)
{
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = data;
}

void ST7789_write_command(uint8_t cmd)
{
    P1OUT &= ~CS;              // Select device (active low)
    P1OUT &= ~DC;              // D/C = 0 (command mode)
    spi_write_byte(cmd);       // Send 0xAE
    P1OUT |= CS;               // Deselect device
}

void ST7789_write_data(uint8_t data)
{
    P1OUT &= ~CS;              // Select device (active low)
    P1OUT |= DC;               // D/C = 1 (command mode)
    spi_write_byte(data);      // Send 0xAE
    P1OUT |= CS;               // Deselect device
}

void ST7789_write_data_16bit(uint16_t data)
{
    ST7789_write_data(data >> 8);
    ST7789_write_data(data & 0xFF);
}

void ST7789_init()
{
    // Tell the ST7789 about the display and whatnot

    // Reset LCD, hold for 10ns as stated in 7.4.5
    P1OUT &= ~RST;
    __delay_cycles(100000);
    P1OUT |= RST;

    ST7789_write_command(ST7789_SWRST);

    ST7789_write_command(ST7789_SLPOUT);

    ST7789_write_command(ST7789_COLMOD);
    ST7789_write_data(0x55);

    ST7789_write_command(ST7789_MADCTL);
    ST7789_write_data(0x00);

    // Display is 240 x 320
    // 239 => 0x00EF
    // 319 => 0x013F
    ST7789_write_command(ST7789_CASET);
    ST7789_write_data_16bit(ST7789_XSTART);
    ST7789_write_data_16bit(ST7789_XSTART + (LCD_WIDTH - 1));

    ST7789_write_command(ST7789_RASET);
    ST7789_write_data_16bit(ST7789_YSTART);
    ST7789_write_data_16bit(ST7789_XSTART + (LCD_HEIGHT - 1));

    ST7789_write_command(ST7789_INVON);
    ST7789_write_command(ST7789_NORON);



    ST7789_write_command(ST7789_DISPON);

}

void ST7789_set_addr(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1)
{
    // Tell what LCD it is writing to
    ST7789_write_command(ST7789_CASET);
    ST7789_write_data_16bit(x0 + ST7789_XSTART);
    ST7789_write_data_16bit(x1 + ST7789_XSTART);

    ST7789_write_command(ST7789_RASET);
    ST7789_write_data_16bit(y0 + ST7789_YSTART);
    ST7789_write_data_16bit(y1 + ST7789_YSTART);

    ST7789_write_command(ST7789_RAMWR);
}

void ST7789_draw_pixel(uint16_t x, uint16_t y, uint16_t colour)
{
    ST7789_set_addr(x, x, y, y);
    ST7789_write_data_16bit(colour);
}

void ST7789_draw_char(uint16_t x, uint16_t y, uint16_t colour, unsigned char letter)
{
    // uses draw_pixel to draw a charecter
    // Draw entire screen in that colour
    // ex
    // int i;
    // for (i = 0; i < 5; i++){
    //    ST7789_draw_char(x + (i * 5), y, ascii[letter][i]);
    // }

    // Takes in X, Y, and colour
    // Used to write text
    // Needs font.h with a ASCII with 96 rows 5 cols
    // EXAMPLES:
        // asci[0][0~4] => 0x00 0x00 0x00 0x00 0x00 => Space
        // asci[1][0~4] => 0x00 0x00 0x5f 0x00 0x00 => !
        // Each value represents a column, each column is 8 rows with the MSB from the top / LSB from bot
}

void ST7789_clear_screen(uint16_t colour)
{
    int i;
    ST7789_set_addr(0, LCD_WIDTH-1, 0, LCD_HEIGHT-1);

    for(i = 0; i < LCD_SIZE; i++){
        ST7789_write_data_16bit(colour);
    }
}

void ST7789_draw_string(uint16_t x, uint16_t y, uint16_t colour, char* m)
{
    // loop something like
    // for (i = 0; greetings[i] != '\0'; i++) {
    //      ST7789_draw_char(x, y, colour, m[i]
    // }
}
int main(void)
{
    msp430_init();
    spi_init();
    ST7789_init();

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
