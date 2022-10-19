#! /usr/bin/python3
from time import sleep
import smbus2
import time

import resethub

resethub.reset_hub()

bus = smbus2.SMBus(1)
time.sleep(1) #wait here to avoid 121 IO Error

#  USB Attach with SMBusRuntime Access Table 275
bus.write_i2c_block_data(0x2d,0xAA ,[0x56 ,0x00])

    
    
