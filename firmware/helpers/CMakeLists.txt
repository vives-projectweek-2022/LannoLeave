add_library(helpers INTERFACE)

target_include_directories(helpers INTERFACE
  ./include
)

target_sources(helpers INTERFACE
  ./command_handler.cpp
  ./graph.cpp
  ./helper_funcs_var.cpp
  ./i2c_master.cpp
  ./spi_slave.cpp
  ./i2c_slave.cpp
)

target_link_libraries(helpers INTERFACE
  pico_stdlib
  hardware_i2c
  hardware_spi

  i2c_slave_lib
)
