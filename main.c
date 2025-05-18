#include <msp430.h> 
#include "drivers/spi.h"
#include "drivers/gmt020-02.h"

/**
 * main.c
 */

int main(void)
{
    msp430_init();
    spi_init();
    ST7789_init();


    // LED P2.1 turns on during execution, and off when not in use
    toggle_led();

    // Command area
    ST7789_clear_screen(BLACK);

    ST7789_draw_string(LCD_WIDTH/2 - 50, LCD_HEIGHT/2, WHITE, "Hello World !");

    
    toggle_led();

    P2OUT &= ~BIT1;

    while (1);
}
