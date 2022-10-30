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
# for enabling hub in cm4
i2ctransfer 1 w3@0x2D 0xaa 0x56 0x00

```
