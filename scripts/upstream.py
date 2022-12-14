#! /usr/bin/python3

from time import sleep
import smbus2
import time
import sys

import smarthub

n = len(sys.argv)
if n < 2:
    print("indicate which port number")
    quit()

port_number = int(sys.argv[1])

#smarthub.reset_hub()

bus = smbus2.SMBus(1)
time.sleep(1)  # wait here to avoid 121 IO Error

smarthub.upstream(bus, port_number)

data=smarthub.retrive_configuration(bus)
print(data)
