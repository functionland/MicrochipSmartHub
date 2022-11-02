# Requirements

```bash

pip install smbus2
pip install RPi.GPIO


#if you test it with yocto image use
pip3 install --trusted-host files.pythonhosted.org --trusted-host pypi.org --trusted-host pypi.python.org smbus2 RPi.GPIO

```

# Run

```bash

# 1-restart hub
python3 resethub.py

# 2- change port upstream
python3 upstream.py <port number>

# 3- start hub in normal state
python3 runhub.py

```

# using i2c-tools

```bash
#for checkin smart hub is enable with address 0x2d
i2cdetect -y 1

# for enabling hub
i2ctransfer -y 1 w3@0x2D 0xaa 0x56 0x00

# example of table 4-2
# write command block for register read
i2ctransfer -y 1 w9@0x2d 0x00 0x00 0x06 0x01 0x02 0xbf 0x80 0x30 0x02
# register configurtion acccess
i2ctransfer -y 1 w3@0x2D 0x99 0x37 0x00
# example smbus read command
i2ctransfer -y 1 w6@0x2D 0x00 0x06 0x5B 0x08 0x16 0x49 r4


```

# Board Network Configuration

```bash
#create shared ip v4 address on usb connection
nmcli connection add type ethernet ifname usb0 ipv4.method shared con-name local

#change ip address range fo usb 
nmcli connection modify local ipv4.address 192.168.25.1/24

nmcli con mod local connection.autoconnect yes

nmcli connection down local
nmcli connection up local

```

