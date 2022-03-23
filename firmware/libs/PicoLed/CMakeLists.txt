add_library(Pico_led INTERFACE)

target_include_directories(Pico_led INTERFACE
  ./include
)

target_sources(Pico_led INTERFACE
  PicoLedController.cpp
  PicoLedEffect.cpp
  PicoLedTarget.cpp
  PioStrip.cpp
  VirtualStrip.cpp
  WS2812B.cpp
)

pico_generate_pio_header(Pico_led ${CMAKE_CURRENT_LIST_DIR}/WS2812B.pio)

target_link_libraries(Pico_led INTERFACE
  hardware_pio
)