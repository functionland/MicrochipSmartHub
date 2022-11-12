#! /usr/bin/python3
from time import sleep
import smbus2
import time

import smarthub

smarthub.reset_hub()

bus = smbus2.SMBus(1)
time.sleep(1)  # wait here to avoid 121 IO Error
while True:
    #smarthub.run_config_reg_access(bus)
    #bus.write_i2c_block_data(
    #    0x2d, 0x00, [0x00, 0x08, 0x00, 0x02, 0xBF, 0x80, 0x30, 0x00, 0x34, 0x12])
    smarthub.run_config_reg_access(bus)
    bus.write_i2c_block_data(
        0x2d, 0x00, [0x00, 0x06, 0x01, 0x02, 0xBF, 0x80, 0x30, 0x00])
    smarthub.run_config_reg_access(bus)
    data=bus.read_i2c_block_data(0x2d,0x00,30)
    # data=bus.read_i2c_block_data(0x2d,0x00,15)
    print("".join("%02x " % b for b in data))
    time.sleep(1)
