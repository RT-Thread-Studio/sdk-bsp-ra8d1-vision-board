# LCD Example
#
# Note: To run this example you will need a LCD Shield for your OpenMV Cam.
#
# The LCD Shield allows you to view your OpenMV Cam's frame buffer on the go.

import sensor
import time
import lcd

sensor.reset()
sensor.set_vflip(True)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
lcd.init()

# FPS clock
clock = time.clock()

while True:
    clock.tick()
    img = sensor.snapshot()
    img.draw_string(0, 0, "fps:%2.2f" % clock.fps(), color=(255,0,0), scale=1)
    lcd.display(img)  # Take a picture and display the image.
