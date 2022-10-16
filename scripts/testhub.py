#! /usr/bin/python

from time import sleep
from smbus2 import SMBus, i2c_msg
import RPi.GPIO as GPIO
import time

# 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1 (port I2C1)
with SMBus(1) as bus:
    # Example 4-2

    reset_pin=10
    # here we should restart hub
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(reset_pin, GPIO.OUT)

    # Reset SmartHub with gpio pins
    for i in range(10):
        GPIO.output(reset_pin, GPIO.HIGH)
        time.sleep(0.2)
        GPIO.output(reset_pin, GPIO.LOW)
        time.sleep(0.2)

    GPIO.cleanup()


    # USB Attach with SMBus Runtime Access
    # In order for SMBus client operation to operate continuously during run-
    # time, the MCU suspend feature of the hub must be disabled by setting
    # the BYPASS_MCU_SUSPEND bit to one. See Table 212 for register details.
    # use 0x00 as termination command delimater
    msg = i2c_msg.write(0x2D, [0xAA, 0x56,0x00])
    bus.i2c_rdwr(msg)

    # To boot hub
    time.sleep(1)

    # TABLE 279: SMBUS WRITE COMMAND BLOCK FOR REGISTER READ
    msg = i2c_msg.write(
        0x2D, [0x00, 0x00, 0x06, 0x01, 0x02, 0xbf, 0x80, 0x30, 0x2,0x00])
    bus.i2c_rdwr(msg)

    # TABLE 280: CONFIGURATION REGISTER ACCESS COMMAND
    msg = i2c_msg.write(0x2D, [0x99, 0x37, 0x00])
    bus.i2c_rdwr(msg)

    # TABLE 281: EXAMPLE SMBUS READ COMMAND
    msg = i2c_msg.write(0x2D, [0x00, 0x06,0x00])
    bus.i2c_rdwr(msg)

    msg = i2c_msg.write(0x2D, [0x08, 0x016, 0x49,0x00])
    bus.i2c_rdwr(msg)

    msg = i2c_msg.read(0x2D, 8)
    bus.i2c_rdwr(msg)
    for k in range(msg.len):
        print(msg.buf[k])
