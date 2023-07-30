#!/bin/sh
# Will build and push code to esp8266 using the
# platformio.ini file as the env source.

. ~/.platformio/penv/bin/activate

# Make sure we have all the libraries needed
pio lib install

# Build and install
pio run -e esp12e -t upload