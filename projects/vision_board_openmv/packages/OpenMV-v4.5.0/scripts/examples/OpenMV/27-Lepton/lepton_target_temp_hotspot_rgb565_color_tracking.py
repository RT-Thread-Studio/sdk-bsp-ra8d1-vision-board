# Single Color RGB565 Blob Tracking Example
#
# This example shows off single color RGB565 tracking using the OpenMV Cam using the FLIR LEPTON.

# By turning the AGC off and setting a max and min temperature range you can make the lepton into
# a great sensor for seeing objects of a particular temperature. That said, the FLIR lepton is a
# microblobometer and not a thermophile. So, it needs to re-calibrate itself often (which is called
# flat-field-correction - FFC). Additionally, microblobmeter devices require pprocessing support
# onboard to deal with the effects of temperature drift which is called radiometry support.

# FLIR Lepton Shutter Note: FLIR Leptons with radiometry and a shutter will pause the video often
# as they heatup to re-calibrate. This will happen less and less often as the sensor temperature
# stablizes. You can force the re-calibration to not happen if you need to via the lepton API.
# However, it is not recommended because the image will degrade overtime.

# If you are using a LEPTON other than the Lepton 3.5 this script may not work perfectly as other
# leptons don't have radiometry support or they don't activate their calibration process often
# enough to deal with temperature changes (FLIR 2.5).

import sensor, image, time, math

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
threshold_list = [( 70, 100,  -30,   40,   20,  100)]

# Set the target temp range here
min_temp_in_celsius = 20
max_temp_in_celsius = 32

print("Resetting Lepton...")
# These settings are applied on reset
sensor.reset()
sensor.ioctl(sensor.IOCTL_LEPTON_SET_MEASUREMENT_MODE, True)
sensor.ioctl(sensor.IOCTL_LEPTON_SET_MEASUREMENT_RANGE, min_temp_in_celsius, max_temp_in_celsius)
print("Lepton Res (%dx%d)" % (sensor.ioctl(sensor.IOCTL_LEPTON_GET_WIDTH),
                              sensor.ioctl(sensor.IOCTL_LEPTON_GET_HEIGHT)))
print("Radiometry Available: " + ("Yes" if sensor.ioctl(sensor.IOCTL_LEPTON_GET_RADIOMETRY) else "No"))
# Make the color palette cool
sensor.set_color_palette(sensor.PALETTE_IRONBOW)

sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=5000)
clock = time.clock()

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. "merge=True" merges all overlapping blobs in the image.

while(True):
    clock.tick()
    img = sensor.snapshot()
    for blob in img.find_blobs(threshold_list, pixels_threshold=200, area_threshold=200, merge=True):
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
    print("FPS %f - Lepton Temp: %f C" % (clock.fps(), sensor.ioctl(sensor.IOCTL_LEPTON_GET_FPA_TEMPERATURE)))
