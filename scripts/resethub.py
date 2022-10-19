from time import sleep
from smbus2 import SMBus, i2c_msg
import RPi.GPIO as GPIO
import time



def reset_hub():
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

    #GPIO.cleanup()


    bus = SMBus(1)
    b = bus.read_byte_data(0x2d, 0)
    print("address 0: ",b)
