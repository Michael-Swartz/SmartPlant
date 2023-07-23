#!/bin/sh
# Will build and push code to esp8266 using the
# platformio.ini file as the env source.

. ~/.platformio/penv/bin/activate

pio run -e esp12e -t upload