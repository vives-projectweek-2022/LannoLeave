add_library(command_handler INTERFACE)

target_include_directories(command_handler INTERFACE
  ./include
)

target_sources(command_handler INTERFACE
  command_handler.cpp
)