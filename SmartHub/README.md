

sudo apt install libusb-1.0-0-dev
sudo apt-get install libssl-dev


./vcpkg install cpp-httplib
openssl req -newkey rsa:2048 -new -nodes -x509 -days 3650 -keyout key.pem -out cert.pem

make -DCMAKE_TOOLCHAIN_FILE=/home/mohsen/vcpkg/scripts/buildsystems/vcpkg.cmake  ../
python3 -m http.server
python -m http.server 8000 --bind 127.0.0.1

What is smart hub?
is a usb hub that integrates system-level functions typically associated with a seprate MCU or Processor.