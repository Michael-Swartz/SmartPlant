import sys
import time
import gpiozero


#One Pump configuration
pump1_pin = 3

pump1 = gpiozero.OutputDevice(pump1_pin, active_high=False, initial_value=False)

while 1:
	pump1.on()
	time.sleep(5)
	pump1.off()
	time.sleep(5)
