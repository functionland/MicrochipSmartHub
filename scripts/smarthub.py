#! /usr/bin/python3

import smbus2
import time
import RPi.GPIO as GPIO

I2C_ADDR = 0x2D
WRITE_CMD = 0x00

# Table1 from datasheet
BASEADDRESS0 = 0xBF800000
HUB_CFG = 0x3006 + BASEADDRESS0

# Table2 from datasheet
BASEADDRESS1 = 0xBFD20000
FLEX_FEATURE_REG = 0x3440 + BASEADDRESS1


def run_config_reg_access(bus):
    # Read and Write Configuration registers

    bus.write_i2c_block_data(I2C_ADDR, 0x99, [0x37, 0x00])


def usb_attach_with_smb_runtime_access(bus):
    #  USB Attach with SMBusRuntime Access Table 275

    bus.write_i2c_block_data(I2C_ADDR, 0xAA, [0x56, 0x00])


def write_reg(bus, reg, values):
    count = len(values)
    reg_bytes = (reg).to_bytes(4, byteorder='big')
    data = [0x00,
            6 + count,
            WRITE_CMD,
            count,
            reg_bytes[0],
            reg_bytes[1],
            reg_bytes[2],
            reg_bytes[3]]
    bus.write_i2c_block_data(I2C_ADDR, 0x00, data+values)


def read_reg(bus, reg, count):
    reg_bytes = (reg).to_bytes(4, byteorder='big')
    bus.write_i2c_block_data(
        I2C_ADDR, 0x00, [0x00,
                         6,
                         1,
                         count,
                         reg_bytes[0],
                         reg_bytes[1],
                         reg_bytes[2],
                         reg_bytes[3]
                         ]
    )
    run_config_reg_access(bus)
    bus.write_i2c_block_data(I2C_ADDR, 0x00, [0x00])
    return bus.read_i2c_block_data(I2C_ADDR, 0x00, count)


def get_generic_reg_value(bus, reg, count):
    value = read_reg(bus, reg, count)
    return value[1:]


def retrive_configuration(bus):
    data = read_reg(bus, FLEX_FEATURE_REG, 3)
    return (data[4] << 24)+(data[3] << 16)+(data[2] << 8) + data[1]


def upstream(bus, port):
    write_reg(bus, HUB_CFG, [port])


def set_flex_feature_reg(bus, value):
    write_reg(bus, FLEX_FEATURE_REG, [value])


def get_flex_feature_reg(bus, value):
    data = read_reg(bus, FLEX_FEATURE_REG, 2)
    return (data[2] << 8) + data[1]


def read_otp(bus, count):
    return bus.read_i2c_block_data(I2C_ADDR, 0x00, count)


def reset_hub():
    reset_pin = 23
    active_usb_pin = 17

    GPIO.setmode(GPIO.BCM)

    GPIO.setwarnings(False)
    print("smarthub is reseting....")

    GPIO.setup(reset_pin, GPIO.OUT)
    GPIO.setup(active_usb_pin, GPIO.OUT)

    GPIO.output(reset_pin, GPIO.LOW)
    GPIO.output(active_usb_pin, GPIO.LOW)
    time.sleep(1)
    GPIO.output(reset_pin, GPIO.HIGH)
    GPIO.output(active_usb_pin, GPIO.HIGH)
    time.sleep(1)

    # GPIO.cleanup()

    time.sleep(1)
    print("smarthub finished reset")
