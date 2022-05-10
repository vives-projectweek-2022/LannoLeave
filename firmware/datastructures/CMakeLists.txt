add_library(datastructures INTERFACE)

target_include_directories(datastructures INTERFACE include/)

target_sources(datastructures INTERFACE
  ./graph.cpp
)

