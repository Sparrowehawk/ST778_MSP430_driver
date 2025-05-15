#include <msp430.h> 
#include "drivers/spi.h"

/**
 * main.c
 */
void spi_init()
{

    UCA0CTL1 |= UCSWRST;                  // Put USCI in reset
    UCA0CTL1 |= UCSSEL_2;                 // Select SMCLK as the clock source

    UCA0CTL0 = UCMST + UCMODE_0 + UCSYNC + UCMSB + UCCKPL;

    // Sets timing for 100KHz as 1MHz (SMLK) / 10 = 100k
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

    BCSCTL1 = CALBC1_16MHZ;      // Set freq to 1MHz
    DCOCTL = CALDCO_16MHZ;

    P2DIR |= BIT1;
    P2OUT &= ~BIT1;
}

void toggle_led()
{
    P2OUT ^= BIT1;
}

void ST7789_write(uint8_t data)
{
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = data;
    while (UCA0STAT & UCBUSY);
}

void ST7789_start_write_data()
{
    P1OUT &= ~CS;
    P1OUT |= DC;
}

void ST7789_start_write_command()
{
    P1OUT &= ~CS;
    P1OUT &= ~DC;
}

void ST7789_end_write()
{
    P1OUT |= CS;
}

void ST7789_write_16bit(uint16_t data)
{
    ST7789_write(data >> 8);
    ST7789_write(data & 0xFF);
}

void ST7789_set_addr(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1)
{
    // Tell what LCD it is writing to
    ST7789_start_write_command();
    ST7789_write(ST7789_CASET);
    ST7789_start_write_data();
    ST7789_write_16bit(x0 + ST7789_XSTART);
    ST7789_write_16bit(x1 + ST7789_XSTART);

    ST7789_start_write_command();
    ST7789_write(ST7789_RASET);
    ST7789_start_write_data();
    ST7789_write_16bit(y0 + ST7789_YSTART);
    ST7789_write_16bit(y1 + ST7789_YSTART);

    ST7789_start_write_command();
    ST7789_write(ST7789_RAMWR);
    ST7789_end_write();
}

void ST7789_draw_pixel(uint16_t x, uint16_t y, uint16_t colour)
{
    ST7789_set_addr(x, x+1, y, y+1);
    ST7789_start_write_data();
    ST7789_write_16bit(colour);
    ST7789_end_write();
}

void ST7789_init()
{
    // Tell the ST7789 about the display and whatnot

    ST7789_start_write_command();

    // Reset LCD, hold for 10ns as stated in 7.4.5
    P1OUT &= ~RST;
    __delay_cycles(1000);
    P1OUT |= RST;
    __delay_cycles(1000);

    ST7789_start_write_command();
    ST7789_write(ST7789_SWRST);
    ST7789_write(ST7789_SLPOUT);

    ST7789_write(ST7789_COLMOD); // 0xBE 0x77
    ST7789_start_write_data();
    ST7789_write(0x55);

    ST7789_start_write_command();
    ST7789_write(ST7789_MADCTL);
    ST7789_start_write_data();
    ST7789_write(0x00);

    // Display is 240 x 320
    // 239 => 0x00EF
    // 319 => 0x013F
    ST7789_start_write_command();
    ST7789_write(ST7789_CASET);
    ST7789_start_write_data();
    ST7789_write_16bit(ST7789_XSTART);
    // 239
    ST7789_write_16bit(0x00EF);

    ST7789_start_write_command();
    ST7789_write(ST7789_RASET);
    ST7789_start_write_data();
    ST7789_write_16bit(ST7789_YSTART);
    ST7789_write_16bit(0x013F);

    ST7789_start_write_command();
    ST7789_write(ST7789_INVON);
    ST7789_write(ST7789_NORON);


    ST7789_write(ST7789_DISPON);
    ST7789_end_write();
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
    uint32_t i;
    ST7789_set_addr(0, LCD_WIDTH-1, 0, LCD_HEIGHT-1);

    ST7789_start_write_data();
    for(i = LCD_SIZE; i > 0; i--){
        ST7789_write_16bit(colour);
    }
    ST7789_end_write();
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
    toggle_led();

    ST7789_clear_screen(BLUE);

    toggle_led();


    P2OUT &= ~BIT1;

    while (1);
}
