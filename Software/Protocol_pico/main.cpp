#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "src/leaf.h"
#include "src/controller.h"
#include "src/commands.h"

#define DEBUG
#define CONTROLLER
#define UNCONFIGUREDADDRESS 0x08
#define GENCALLADR 0x00

using namespace LannoLeaf;

#ifdef CONTROLLER
  LannoLeaf::Controller controller((uint8_t) UNCONFIGUREDADDRESS, i2c0);
#else
  LannoLeaf::Leaf leaf((uint8_t) UNCONFIGUREDADDRESS, i2c0);  
#endif

#ifdef DEBUG
  void set_alive_led(void) {
    const uint led_pin = 25;
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_put(led_pin, true);
  }
#endif

int main() {
  stdio_init_all();
  
  #ifdef DEBUG
    set_alive_led();
    sleep_ms(5000);
    printf("Starting platform\r\n");
  #endif

  #ifdef CONTROLLER
    controller.initialize();
    controller.search_topology();
  #else
    leaf.initialize();
  #endif

  while (true) {
    #ifdef CONTROLLER
      controller.update();
    #else
      leaf.update();
    #endif
  }
}
