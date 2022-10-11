#! /usr/bin/python

from smbus2 import  SMBus, i2c_msg

# 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1 (port I2C1)
with SMBus(1) as bus:
    # Read 64 bytes from address 80
    # Example 4-2

    # TABLE 279: SMBUS WRITE COMMAND BLOCK FOR REGISTER READ
    msg=i2c_msg.write(0x2D, [0x00,0x00,0x06,0x01,0x02,0xbf,0x80,0x30,0x2])
    bus.i2c_rdwr(msg)

    # TABLE 280: CONFIGURATION REGISTER ACCESS COMMAND
    msg=i2c_msg.write(0x2D, [0x99,0x37,0x00])
    bus.i2c_rdwr(msg)

    # TABLE 281: EXAMPLE SMBUS READ COMMAND
    msg=i2c_msg.write(0x2D, [0x00,0x06])
    bus.i2c_rdwr(msg)

    msg=i2c_msg.read(0x2D, [0x08,0x016,0x49])
    bus.i2c_rdwr(msg)
    print(msg)


