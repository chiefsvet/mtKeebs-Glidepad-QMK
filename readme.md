# Cirque Trackpad RP2040 (handwired)

A hand-wired USB-C trackpad built around a Waveshare RP2040 board and a
35mm Cirque GlidePoint (Pinnacle 1CA027, SPI) circle trackpad module, with
two momentary hardware buttons as a fallback for left/right click.

* MCU: Waveshare RP2040 (Zero-style board; USB-C wired directly to a host,
  no wireless)
* Pointing device: Cirque Pinnacle / GlidePoint, 35mm, SPI mode
* Two direct-wired momentary switches for left/right click

## Status

Confirmed working as of this build:
- Cursor tracking, correct axis orientation (180-degree rotation applied
  in `config.h` -- adjust if you rebuild for a different physical mount)
- Tap-to-click (left click)
- Circular/perimeter scroll (touch the outer ring and drag along the edge)
- Hardware left/right click buttons (GP6/GP7) are wired into the firmware
  but as of this writing haven't been physically wired into the build yet

## Wiring

| Signal              | RP2040 pin | Goes to                          |
| -------------------- | ---------- | --------------------------------- |
| SPI SCK               | GP2        | Trackpad SCK                      |
| SPI MOSI               | GP3        | Trackpad MOSI (DIN)                |
| SPI MISO               | GP4        | Trackpad MISO (DOUT)               |
| Chip Select            | GP5        | Trackpad CS/SS                    |
| Left click switch      | GP6        | One leg here, other leg -> GND     |
| Right click switch     | GP7        | One leg here, other leg -> GND     |
| 3.3V                   | 3V3        | Trackpad VCC                      |
| GND                    | GND        | Trackpad GND, both switches' other leg |
| DR (Data Ready)         | *not connected* | See note below                    |

Notes:
- **DR is intentionally left unconnected.** The Cirque Pinnacle has a
  hardware Data-Ready pin meant to interrupt the host when new touch data
  is available, but QMK's `cirque_pinnacle_spi` driver doesn't use it —
  instead it polls the sensor over SPI on a timer (every 10ms by default,
  via `POINTING_DEVICE_TASK_THROTTLE_MS`) and checks a *software*
  data-ready bit in a status register. Since the Pinnacle only updates its
  position data every 10ms internally regardless, polling gets you the
  same responsiveness without the extra wire. If your specific breakout
  board's datasheet says DR needs to be tied to a particular level to
  behave, follow that — but on most Cirque circle trackpad FFC breakouts
  it's safe to leave floating/unconnected in this polling setup.
- The Cirque module runs at 3.3V logic — the RP2040's GPIO/3V3 rail matches,
  so no level shifting is needed.
- The two buttons use QMK's `direct` matrix pins, so no diodes are required
  — just wire each switch between its GPIO and GND. Internal pull-ups are
  enabled automatically.
- If your board's silkscreen GPIO numbers differ from the ones above (very
  possible on the compact RP2040-Zero form factor), edit the pin defines in
  `config.h` to match how you actually soldered it — the numbers here are a
  starting point, not a hard requirement.

## Building

From the root of your `qmk_firmware` checkout, copy this
`handwired/cirque_trackpad_rp2040` folder into `keyboards/handwired/`, then:

```sh
qmk compile -kb handwired/cirque_trackpad_rp2040 -km default
```

This produces a `.uf2` file.

## Flashing

1. Hold the **BOOT/BOOTSEL** button on the RP2040 board, plug it into your
   computer via USB-C, then release BOOT. It will mount as a USB mass
   storage drive (e.g. `RPI-RP2`).
2. Copy the compiled `.uf2` file onto that drive. The board will reboot
   automatically running the new firmware.

You can also use `qmk flash -kb handwired/cirque_trackpad_rp2040 -km default`
while the board is in bootloader mode.

## First bring-up tips

- If the cursor doesn't move at all, double check the SPI wiring (SCK/MOSI/
  MISO/CS) and that `CIRQUE_PINNACLE_SPI_CS_PIN` in `config.h` matches the
  pin you wired CS to.
- If the cursor moves but in the wrong direction, or diagonally instead of
  straight, try the `POINTING_DEVICE_ROTATION_*` / `POINTING_DEVICE_INVERT_*`
  defines commented out in `config.h`.
- Temporarily add `#define CIRQUE_PINNACLE_DEBUG` to `config.h` and open the
  QMK console (`qmk console`) to see raw driver output while debugging.
- The two hardware buttons work independently of the trackpad sensor, so if
  the Cirque module ever isn't detected/initializing correctly, left/right
  click on GP6/GP7 will still function as a fallback, per your requirement.
