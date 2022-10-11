#! /usr/bin/python

from time import sleep
from smbus2 import SMBus, i2c_msg

# 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1 (port I2C1)
with SMBus(1) as bus:
    # Example 4-2

    # USB Attach with SMBus Runtime Access
    # In order for SMBus client operation to operate continuously during run-
    # time, the MCU suspend feature of the hub must be disabled by setting
    # the BYPASS_MCU_SUSPEND bit to one. See Table 212 for register details.
    msg = i2c_msg.write(0x2D, [0xAA, 0x56])
    bus.i2c_rdwr(msg)

    # To boot hub
    sleep(1)

    # TABLE 279: SMBUS WRITE COMMAND BLOCK FOR REGISTER READ
    msg = i2c_msg.write(
        0x2D, [0x00, 0x00, 0x06, 0x01, 0x02, 0xbf, 0x80, 0x30, 0x2])
    bus.i2c_rdwr(msg)

    # TABLE 280: CONFIGURATION REGISTER ACCESS COMMAND
    msg = i2c_msg.write(0x2D, [0x99, 0x37, 0x00])
    bus.i2c_rdwr(msg)

    # TABLE 281: EXAMPLE SMBUS READ COMMAND
    msg = i2c_msg.write(0x2D, [0x00, 0x06])
    bus.i2c_rdwr(msg)

    msg = i2c_msg.write(0x2D, [0x08, 0x016, 0x49])
    bus.i2c_rdwr(msg)

    msg = i2c_msg.read(0x2D, 8)
    bus.i2c_rdwr(msg)
    for k in range(msg.len):
        print(msg.buf[k])
