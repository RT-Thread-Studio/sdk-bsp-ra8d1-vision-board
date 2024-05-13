# This work is licensed under the MIT license.
# Copyright (c) 2013-2023 OpenMV LLC. All rights reserved.
# https://github.com/openmv/openmv/blob/master/LICENSE
#
# Key irq example
#
# When the onboard USER BOOT button is pressed, user key pressed information will be printed.

from machine import Pin
import time

PIN_KEY0 = 0x907
key_0 = Pin(("key_0", PIN_KEY0))
key_flag = 0

def func(v):
    global key_flag
    key_flag = 1

key_0.irq(trigger=Pin.IRQ_RISING, handler=func)

while True:
    if key_flag == 1:
        key_flag = 0
        print("user key pressed")
