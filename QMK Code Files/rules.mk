# No BOARD override here on purpose. Leaving BOARD unset makes QMK use its
# default RP2040 profile (matching the Sparkfun Pro Micro RP2040 pinout),
# which already comes with SPI0 fully configured. An earlier version of
# this file set BOARD = GENERIC_RP_RP2040 (the "bare", nothing-preconfigured
# RP2040 profile) which required hand-written mcuconf.h/halconf.h files to
# re-enable SPI -- that path never fully worked, so it's been removed in
# favor of just using the default profile like the original working
# ScottoCirque firmware does.

# --- Pointing device (Cirque GlidePoint 35mm, SPI) ---
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = cirque_pinnacle_spi

# --- TEMPORARY: debug console, remove once scroll is confirmed working ---
CONSOLE_ENABLE = yes
