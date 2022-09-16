



```bash

# getting required packages on board side
sudo apt install libusb-1.0-0-dev i2c-tools libssl-dev libi2c-dev

# install required packages for build project
./vcpkg install cpp-httplib


#build project
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake  ../
make -j4


#generating digital signiture for https
openssl req -newkey rsa:2048 -new -nodes -x509 -days 3650 -keyout key.pem -out cert.pem

#run simple web server to test web hooks
python3 -m http.server
python -m http.server 8000 --bind 127.0.0.1

```
