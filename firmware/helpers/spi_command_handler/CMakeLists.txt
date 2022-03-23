add_library(spi_command_handler INTERFACE)

target_include_directories(spi_command_handler INTERFACE
  ./include
)

target_sources(spi_command_handler INTERFACE
  spi_command_handler.cpp
)

target_link_libraries(spi_command_handler INTERFACE
  hardware_spi
)