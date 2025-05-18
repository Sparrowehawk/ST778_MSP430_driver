# SPI Driver for MSP430G2553 for a ST7789 chip using the GMT020-02

An example of how to use a ST7789 to draw on any display which uses it via SPI. The display used in this is the GMT020-02, found on AliExpress for ~£2

## Parts Used
- MSP430G2553
- GMT020-02 (with a ST7789 chip)
- Logic Analyzer (used to see the communications)

## Setup

1. **Wiring:**
   - Connect the relavent power pins, note that the SPI pins have to be to the relevant pins on the MSP430; however, the rest can be set to any GPIO
     - `P1.1` — SPI Reciever (MISO) ——— Not used, useful to know
     - `P1.2` — SPI Transmitter (MOSI)
     - `P1.4` — SPI Clock (SCLK)
     - `P1.0` — Chip select (CS)
     - `P1.1` — Reset (RST)
     - `P1.3` — Directional control (DC) ——— Used to dictate if selecting or reading a register
      
2. **Debugging:**
   - Use a logic analyzer during SPI setup to ensure that the settings are correct
   - The onboard LED can be used to show when data transfer is taking place
   - Use the display to see if things work
   - If nothing displays, reduce clock speed to 1MHz and UCA0BR0 to 10
    

## Further Improvements

- Add a dirty buffer where it keeps track of bits changed to what value; therefore when a new write is instructed, the screen can keep pixels that do not change the same
  - Can be used to speed up writing to the display
  - Be cautious of the limited RAM on the MSP430G2553
- Connect the display via a veroboard to increase SMCLK
  - Current issue limiting my implementation to ~270KHz is the noise caused via the breadboard

---

## License
MIT License

---

## Author
[Sparrowehawk](https://github.com/Sparrowehawk)
