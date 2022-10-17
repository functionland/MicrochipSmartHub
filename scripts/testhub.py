#! /usr/bin/python
from time import sleep
import smbus2
import RPi.GPIO as GPIO
import time


reset_pin=23
active_usb_pin=17

GPIO.setmode(GPIO.BCM)

GPIO.setwarnings(False)
 
GPIO.setup(reset_pin, GPIO.OUT)
GPIO.setup(active_usb_pin, GPIO.OUT)

print("reset hub")
GPIO.output(reset_pin, GPIO.LOW)
GPIO.output(active_usb_pin, GPIO.LOW)
time.sleep(1)
GPIO.output(reset_pin, GPIO.HIGH)
GPIO.output(active_usb_pin, GPIO.HIGH)
time.sleep(1)

bus = smbus2.SMBus(1)
time.sleep(1) #wait here to avoid 121 IO Error
while True:
    bus.write_i2c_block_data(0x2d,0x99 ,[0x33 ,0x00])
    bus.write_i2c_block_data(0x2d,0x00 ,[0x00 ,0x06 ,0x01 ,0x01 ,0xBF ,0xD2 ,0x34 ,0x19])
    bus.write_i2c_block_data(0x2d,0x99 ,[0x37 ,0x00])
    data=bus.read_i2c_block_data(0x2d,0x00,15);
    print("".join("%02x " % b for b in data))
    time.sleep(1)
    
    
