# This work is licensed under the MIT license.
# Copyright (c) 2013-2023 OpenMV LLC. All rights reserved.
# https://github.com/openmv/openmv/blob/master/LICENSE
#
# UART Control
#
# This example shows how to use the serial port on your OpenMV Cam. Attach pin
# P802 ---> UART2_RX
# P801 ---> UART2_TX

import time
from pyb import UART

uart = UART(2, 115200, timeout_char=200)

str_buffer = "Hello World"

while True:
    uart.write(str_buffer)

    buf = uart.read(len(str_buffer)).decode('utf-8')
    if buf is not '':
        print("UART2 receive data:", buf)
    else:
        print("UART2 did not receive any data")

    time.sleep_ms(1000)
