add_library(pixel_mapping INTERFACE)

target_include_directories(pixel_mapping INTERFACE
  ./include
)

target_link_libraries(pixel_mapping INTERFACE
  graph
)

target_sources(pixel_mapping INTERFACE
  pixel_mapping.cpp
)
