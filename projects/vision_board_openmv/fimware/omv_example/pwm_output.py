# This work is licensed under the MIT license.
# Copyright (c) 2013-2023 OpenMV LLC. All rights reserved.
# https://github.com/openmv/openmv/blob/master/LICENSE
#
# PWM Control Example
#
# This example shows how to do PWM with your OpenMV Cam.

from machine import PWM 

pwm = PWM(13, 0, 100 * 1000, 100)   # The initialized frequency is 100KHz and the duty cycle value is 100 (duty cycle is 100/255 = 39.22%)
pwm.duty(200)                       # Set the PWM object duty cycle value