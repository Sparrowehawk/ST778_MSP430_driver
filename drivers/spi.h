/*
 * spi.h
 *
 *  Created on: 14 May 2025
 *      Author: Hamid
 */

#ifndef DRIVERS_SPI_H_
#define DRIVERS_SPI_H_

#include <stdint.h>
#include <stdbool.h>

#define SCLK BIT4
#define MOSI BIT2
// No MiSO in this driver
#define CS   BIT0
#define RST  BIT1
#define DC   BIT3

#define ST7789_XSTART 0x0000
#define ST7789_YSTART 0x0000

#define ST7789_SWRST  0x01
#define ST7789_SLPOUT 0x11
#define ST7789_COLMOD 0x3A
#define ST7789_MADCTL 0x36
#define ST7789_CASET  0x2A
#define ST7789_RASET  0x2B
#define ST7789_INVON  0x21
#define ST7789_NORON  0x13
#define ST7789_RAMWR  0x2C
#define ST7789_DISPON 0x29

// dispay is 240 x 320
#define LCD_WIDTH  ((uint16_t)240)
#define LCD_HEIGHT ((uint16_t)320)
#define LCD_SIZE   ((uint32_t)LCD_WIDTH * LCD_HEIGHT)

#define BLACK  0x0000
#define WHITE  0xFFFF
#define RED    0xF800
#define GREEN  0x07E0
#define BLUE   0x001F
#define CYAN   0x07FF
#define YELLOW 0xFFC0
#define PURPLE 0xF81F
#define PINK   0xD252

void spi_init();
void msp430_init();
void ST7789_init();
void toggle_led();

void ST7789_write(uint8_t data);

void ST7789_start_write_command();
void ST7789_start_write_data();
void ST7789_write_16bit(uint16_t data);
void ST7789_end_write();

void ST7789_set_addr(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1);
void ST7789_draw_pixel(uint16_t x, uint16_t y, uint16_t colour);

void ST7789_draw_char(uint16_t x, uint16_t y, uint16_t colour, unsigned char letter);
void ST7789_clear_screen(uint16_t colour);
void ST7789_draw_string(uint16_t x, uint16_t y, uint16_t colour, char* m);


#endif /* DRIVERS_SPI_H_ */
