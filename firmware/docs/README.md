# Compiling project

## Requirements
- Cmake (version >=3.13)
- gcc-arm-none-eabi
- build essentials
- [pico_sdk](https://github.com/raspberrypi/pico-sdk)

```bash

sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential

```

### Recomended
- CMake Tools VS Code extention where you can simply build by pressing one [button](https://bitly.com/98K8eH)

<img src="VSCode_CMake.png">

## Build

```bash

mkdir build && cd build
cmake ..

# Building
cd build
cmake --build .

```

## Documentation
Doxygen documentation will be generated on build, and will be put inside docs folder as doxy.

## SDK documentation

- [Pi-pico-c-sdk](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
