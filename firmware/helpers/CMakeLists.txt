add_library(helpers INTERFACE)

target_include_directories(helpers INTERFACE
  ./include
)

target_sources(helpers INTERFACE
  ./helper_funcs_var.cpp
)

target_link_libraries(helpers INTERFACE
  pico_stdlib
  pico_util
  hardware_i2c
  hardware_spi

  baseclasses
  i2c_slave_lib
)
