## mtKeebs Glidepad

The mtKeebs Glidepad is my version of the ScottoCirque trackpad. Check out Joe Scotto's [YouTube video](https://www.youtube.com/watch?v=AhoCn2nf1GU) and his [website](https://scottokeebs.com/blogs/macropads/scottocirque) for more information. He calls himself "the handwiring guy" because the majority of his builds are hand-wired, although he's been doing more PCB builds, too. If you're new to electronics and custom mechanical keyboards like me, check out his YT channel. I learned 99% of what I know from his channel.

The ScottoCirque uses a 40 mm trackpad with a flat overlay, but I wanted to use a curved overlay. The 40 mm size was on backorder everywhere I've looked, so I purchased the 35 mm version. I originally put the trackpad into a custom hand-wired mechanical keyboard that I was already in the process of designing/building at the time his video came out that was using CircuitPython, but I was having a lot of issues with implementation of the trackpad driver. I was able to get the cursor to move fairly accurately, but tap to click was incredibly unreliable (registering only 1 in 10 taps) and I couldn’t get scrolling to work. I also tried using ZMK firmware for the trackpad, but was having a lot of issues with tracking directionality and again I couldn’t get scrolling to work. I will eventually go back and try ZMK again since I really prefer to have a wireless option for connecting keyboards to my computer.

After these two failures, I decided to start from scratch based off Joe Scotto’s version using a wired build and QMK. Like he states in his video about his version, the design here is just for testing purposes. It will eventually be built into a custom keyboard with better buttons for left and right click. 

**Some changes in my build compared to the ScottoCirque:**
1. 35 mm trackpad with a curved overlay
2. I had a Waveshare RP2040 on hand so I used that board instead
3. I didn’t want screws visible form the top so I designed my own case with screws inserting from the bottom
4. There are two small holes over the boot and reset buttons for access while the case is closed. The RP2040 is mounted upside down so the buttons face the bottom of the case.
4. Since I was having trouble with tap to click previously, I added two momentary push buttons to use as backup for left and right click. Turns out that was a wise choice because I still struggle with tap to click. I’ll redesign them when this goes into a keyboard. 
5. I still can’t get scrolling to work just using my finger on the trackpad, so I had Claude.ai create custom code for the scrolling behavior and made the right click button have a press and hold feature to get the scroll feature to work. If I still can't scrolling to work without it, I'll probably add a third (center) button that I can press with my thumb and use my index finger to scroll on the trackpad.


### Bill of Materials

| Part                                                                                                                                                                                 | Qty | Notes                                                                                                                                                                                                                     |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | --- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [Waveshare RP2040-Zero](https://www.waveshare.com/rp2040-zero.htm?srsltid=AfmBOorBt2v1roA5Ej6xlDoV228nK-XgI2wQrG1-7OLUrxRTVEoYe3wA) or similar third-party board                     | 1   | RP2040, wired only, physical boot loader & reset buttons on board                                                                                                                                                         |
| [Cirque Glidepoint Trackpad](https://www.mouser.com/en/ProductDetail/Cirque/TM035035-2024-003?qs=sGAEpiMZZMu3sxpa5v1qrmePy6bg6o9msS9wwvLw9t0%3D), 35mm circular, curved overlay, SPI | 1   | Be sure to get the version with the curved overlay and that there's a resistor at R1 for **SPI** not I2C (required by this build). The case is designed for the version with the curved overlay and not the flat overlay. |
| [Tactile Push Button Micro Momentary Switches](https://www.amazon.com/dp/B09R3ZPWJ7?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1), 6x6x6 mm, 2-pin                                         | 4   | For left & right click buttons, soft\\\_off power, and reset                                                                                                                                                              |
| [M2 Heat Set Insert](https://www.amazon.com/dp/B0CS6XJSSL?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_2&th=1)                                                                         | 4   | -                                                                                                                                                                                                                         |
| [M2 Phillips Flathead Screws](https://www.amazon.com/dp/B082XXW76M?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_7&th=1), 6mm                                                           | 4   | -                                                                                                                                                                                                                         |
| [3M Double-Sided Tape](https://www.amazon.com/dp/B0DJNVVPY1?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1&th=1), 1/2-inch  | 1   | Be sure yours is compatible with electronics. Only a small piece is needed to secure RP2040 to the case.  |
| [28 AWG Stranded Hook Up Wire](https://www.amazon.com/dp/B073RDKRBX?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_8&th=1)   | *   | Various colors & lengths                                                                                                                                                                                                  |
| Soldering iron & solder                                                                                                                                                              | 1   | -                                                                                                                                                                                                                         |


### Wiring Guide

| Signal               | RP2040             | Connects to...
| -------------------- | ------------------ | ------------------------------------------ |
| SPI SCK              | GP2                | SCK on trackpad                            |
| SPI MOSI             | GP3                | SI on trackpad                             |
| SPI MISO             | GP4                | SO on trackpad                             |
| Chip Select          | GP5                | SS (same as CS for other manufacturers)    |
| 3.3V                 | 3v3                | VDD                                        |
| GND                  | GND                | GND                                        |
| DR (Data Ready)      | *not connected*    | Not needed in this build                   |
| Left button click    | GP6                | One pin of left momentary button           |
| Left button ground   | GND                | Other pin of left momentary button         |
| Right button click   | GP7                | One pin of right momentary button          |
| Right button ground  | GND                | Other pin of right momentary button        |

**NOTES**
- The RP2040-Zero only has one ground pin so the 3 ground wires from the trackpad and the two momentary buttons need to be tied together and soldered to the ground pin on the RP20240.
- "CS" and "SS" are the same signal — different vendors label it differently.
- The trackpad's bus-mode jumper (R1, 470kΩ) must be populated for SPI. No resistor there means the board is in I2C mode and the firmware in this repo won’t work. Trackpad came pre-populated with the resistor so it’s in SPI mode. If you need to use I2C,  you’ll have to remove the resistor and rewrite the code.





