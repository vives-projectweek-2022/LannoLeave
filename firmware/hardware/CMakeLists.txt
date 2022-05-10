add_library(hardware INTERFACE)

target_include_directories(hardware INTERFACE include/)

target_sources(hardware INTERFACE
  ./i2c_master.cpp
  ./i2c_slave.cpp
  ./spi_slave.cpp
)

target_link_libraries(
  baseclasses
)
