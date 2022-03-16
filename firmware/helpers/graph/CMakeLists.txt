add_library(graph INTERFACE)

target_include_directories(graph INTERFACE
  ./include
)

target_sources(graph INTERFACE
  graph.cpp
)