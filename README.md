# MicrochipSmartHub


## What is smart hub?

SmartHub Is a usb hub that integrates system-level functions typically associated with a seprate MCU or Processor.

## Building

```bash

# getting required packages on board side
sudo apt install libusb-1.0-0-dev i2c-tools libssl-dev libi2c-dev ninja-build

# install required packages for build project
cd ~/
git clone https://github.com/microsoft/vcpkg
cd vcpkg && ./ bootstrap-vcpkg.sh
./vcpkg install cpp-httplib


# build project
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake  ../
ninja
./SmartHub/MicrochipSmartHub

# For Testing i2c on pc with fake i2c
sudo modprobe i2c-dev
sudo modprobe i2c-stub chip_addr=0x2D
i2cdetect -l
sudo chmod 777 /dev/i2c-x //x is created i2c on device

#generating digital signiture for https
openssl req -newkey rsa:2048 -new -nodes -x509 -days 3650 -keyout key.pem -out cert.pem

#run simple web server to test web hooks
python3 -m http.server
python -m http.server 8000 --bind 127.0.0.1

```