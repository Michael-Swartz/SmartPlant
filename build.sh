#!/bin/sh
# Will build and push code to esp8266 using the
# platformio.ini file as the env source.

# Ensure we have permissions to the USB device
sudo chmod a+rw /dev/ttyUSB0

# Source the Platform IO venv
. ~/.platformio/penv/bin/activate

# Make sure we have all the libraries needed
pio lib install

# Build and install
pio run -e esp32doit-devkit-v1 -t upload