add_executable(controller
  controller.cpp
  main.cpp
)

pico_enable_stdio_usb(controller 1)
pico_enable_stdio_uart(controller 0)

pico_add_extra_outputs(controller)

target_include_directories(controller 
  PRIVATE
  ./include
)

target_link_libraries(controller
  pico_stdlib
  pico_multicore
  Pico_led

  helpers
)

target_compile_definitions(controller PRIVATE
  $<$<CONFIG:Debug>:
    DEBUG=1
  >
)
