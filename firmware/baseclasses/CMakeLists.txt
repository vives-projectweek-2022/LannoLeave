add_library(baseclasses INTERFACE)

target_sources(baseclasses INTERFACE
  ./communication/buffered_communicator.cpp
  ./hardware/i2c.cpp
  ./hardware/spi.cpp
  ./handlers/packet_handler.cpp
  ./handlers/command_handler.cpp
)

target_include_directories(baseclasses INTERFACE
  ./communication/include
  ./hardware/include
  ./handlers/include
)

