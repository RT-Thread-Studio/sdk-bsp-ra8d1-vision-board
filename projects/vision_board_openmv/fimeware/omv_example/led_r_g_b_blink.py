# LED Control Example
#
# This example shows how to control your OpenMV Cam's built-in LEDs. Use your
# smart phone's camera to see the IR LEDs.

import time
from pyb import LED

red_led = LED(1)
green_led = LED(2)
blue_led = LED(3)

while True:
    for led in [red_led, green_led, blue_led]:
        led.on()
        time.sleep_ms(1000)
        led.off()
