# Lannooleaf firmware

## Building the project

**Info**: The installation guide will only go over how to install tools on a debian based linux system. To build on other platforms you will have to figure that out ;).

### Requirements
- Cmake (Version >= 3.13)
- gcc-arm-none-eabi (Version used 10.2.1)
- Linux build-essentials
- Raspberry Pi Pico [sdk](https://github.com/raspberrypi/pico-sdk)
- (Optional) Visual Studio Code CMake Tools [extention](https://code.visualstudio.com/docs/cpp/cmake-linux)

#### Installing requirements

```bash
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essentials
```

**Pico-sdk**: For installation of Raspberry Pi Pico sdk reffer to there [github page](https://github.com/raspberrypi/pico-sdk)

### Building

```bash
cd /PATH/TO/Lannooleaf/firmware/
```

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Or use Visual Studio Code CMake extention to build project with a single press of a [button](https://bitly.com/98K8eH)

## Building documentation

This project automatacly generates API documentation using Doxygen and Sphinx.

If you do not want to use Sphinx, then the doxygen generated html files will be placed in /Lannooleaf/firmware/docs/Doxygen/gen_docs/html folder.

In order to build the Sphinx documentation some tools are required and have to be installed.

```bash
sudo apt-get install -y python3-sphinx
sudo apt-get install -y python3-breathe

python -m pip install exhale

pip install sphinx-book-theme
```

Once tools are installed go to the /docs/Sphinx directory and run.

```bash
make html
```

Html files will be generated an placed in /build/html folder

### Raspberry Pi Pico SDK documentation

All of Rasperry Pi Pico SDK in well documented in there [documentation](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
