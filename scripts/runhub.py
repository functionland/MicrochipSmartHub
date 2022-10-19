#! /usr/bin/python3
from time import sleep
import smbus2
import time

import smarthub

bus = smbus2.SMBus(1)
time.sleep(1) #wait here to avoid 121 IO Error

smarthub.usb_attach_with_smb_runtime_access(bus)

    
    
