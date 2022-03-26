add_library(spi_slave INTERFACE)

target_include_directories(spi_slave INTERFACE
  ./include
)

target_sources(spi_slave INTERFACE
  spi_slave.cpp
)

target_link_libraries(spi_slave INTERFACE
  hardware_spi
)