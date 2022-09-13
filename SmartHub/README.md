
```bash

# getting required packages on board side
sudo apt install libusb-1.0-0-dev
sudo apt-get install libssl-dev

# install required packages for build project
./vcpkg install cpp-httplib
openssl req -newkey rsa:2048 -new -nodes -x509 -days 3650 -keyout key.pem -out cert.pem

#build project
cmake -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake  ../

#run simple web server to test web hooks
python3 -m http.server
python -m http.server 8000 --bind 127.0.0.1

```

What is smart hub?
is a usb hub that integrates system-level functions typically associated with a seprate MCU or Processor.