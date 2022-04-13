add_executable(lannooleaf
  leaf.cpp
  controller.cpp
  main.cpp
)

pico_enable_stdio_usb(lannooleaf 1)

pico_add_extra_outputs(lannooleaf)

target_include_directories(lannooleaf 
  PRIVATE
  ./include
)

target_link_libraries(lannooleaf
  pico_stdlib
  pico_multicore
  Pico_led

  helpers
)

target_compile_definitions(lannooleaf PRIVATE
  $<$<CONFIG:Debug>:
    DEBUG=1
  >
)
