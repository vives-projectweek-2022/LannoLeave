add_executable(leaf 
  leaf.cpp
  main.cpp
)

pico_enable_stdio_usb(leaf 1)
pico_enable_stdio_uart(leaf 0)

pico_add_extra_outputs(leaf)

target_link_libraries(leaf
  pico_stdlib
  pico_multicore

  Pico_led

  helpers
)

target_include_directories(leaf
  PRIVATE
  ./include
)

target_compile_definitions(controller PRIVATE
  $<$<CONFIG:Debug>:
    DEBUG=1
  >
)
