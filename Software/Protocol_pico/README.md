# Compiling project

## Requirements
- Cmake (version >=3.12)
- gcc-arm-none-eabi
- build essentials
- [pico_sdk](https://github.com/raspberrypi/pico-sdk)

```bash
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential
```

### Recomended
- CMake Tools VS Code extention 

## Build

```bash
# Initialisation
mkdir build && cd build
cmake ..

# Building
cd build
cmake --build .
```

Or use vs code plugin set compiler to arm-none-eabi, click on CMake: [Debug] and select Debug on dropdown. 
Build using Build button.

![img](./img/VSCode_CMake.png)