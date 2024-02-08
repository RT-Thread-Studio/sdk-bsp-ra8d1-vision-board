# Deep Sleep Mode Example
# This example demonstrates the low-power deep sleep mode plus sensor shutdown.
# Note the camera will reset after wake-up from deep sleep. To find out if the cause of reset
# is deep sleep, call the machine.reset_cause() function and test for machine.DEEPSLEEP_RESET
import pyb, machine, sensor

# Create and init RTC object.
rtc = pyb.RTC()

# (year, month, day[, hour[, minute[, second[, microsecond[, tzinfo]]]]])
rtc.datetime((2014, 5, 1, 4, 13, 0, 0, 0))

# Print RTC info.
print(rtc.datetime())

sensor.reset()

# Enable sensor softsleep
sensor.sleep(True)

# Optionally bypass the regulator on OV7725
# for the lowest possible power consumption.
if (sensor.get_id() == sensor.OV7725):
    # Bypass internal regulator
    sensor.__write_reg(0x4F, 0x18)

# Shutdown the sensor (pulls PWDN high).
sensor.shutdown(True)

# Enable RTC interrupts every 30 seconds.
# Note the camera will RESET after wakeup from Deepsleep Mode.
rtc.wakeup(30000)

# Enter Deepsleep Mode.
machine.deepsleep()
