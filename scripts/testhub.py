#! /usr/bin/python3
from time import sleep
import smbus2
import time

import resethub

resethub.reset_hub()

bus = smbus2.SMBus(1)
time.sleep(1) #wait here to avoid 121 IO Error
while True:
    bus.write_i2c_block_data(0x2d,0x99 ,[0x33 ,0x00])
    bus.write_i2c_block_data(0x2d,0x00 ,[0x00 ,0x06 ,0x01 ,0x01 ,0xBF ,0xD2 ,0x34 ,0x19])
    bus.write_i2c_block_data(0x2d,0x99 ,[0x37 ,0x00])
    data=bus.read_i2c_block_data(0x2d,0x00,15);
    print("".join("%02x " % b for b in data))
    time.sleep(1)
