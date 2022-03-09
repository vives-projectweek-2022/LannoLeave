add_executable(leaf 
  leaf.cpp
  main.cpp
)

pico_enable_stdio_usb(leaf 1)
pico_enable_stdio_uart(leaf 0)

pico_add_extra_outputs(leaf)

target_link_libraries(leaf
  pico_stdlib
  hardware_i2c
  hardware_irq
  i2c_slave
  helper
  Pico_led
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
