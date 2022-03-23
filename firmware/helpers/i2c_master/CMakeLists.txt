add_library(i2c_master INTERFACE)

target_include_directories(i2c_master INTERFACE
  ./include
)

target_sources(i2c_master INTERFACE
  i2c_master.cpp
)

target_link_libraries(i2c_master INTERFACE
  hardware_i2c
  helper
)