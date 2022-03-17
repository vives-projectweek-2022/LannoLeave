add_library(i2c_master INTERFACE)

target_include_directories(graph INTERFACE
  ./include
)

target_sources(graph INTERFACE
  i2c_master.cpp
)

target_link_libraries(i2c_master INTERFACE
  hardware_i2c
  helper
)