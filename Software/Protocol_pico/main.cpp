#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "src/leaf.h"
#include "src/controller.h"
#include "src/commands.h"

#define DEBUG
#define CONTROLLER
#define I2CADDRESS 0x34

#ifdef CONTROLLER
  LannoLeaf::Controller controller;
#else
  LannoLeaf::Leaf leaf((uint8_t) I2CADDRESS);  
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
    
    #ifdef CONTROLLER
      printf("Scanniing for i2c devices\r\n");
      controller.scan_i2c_devices();

      if (!controller.connected_devices.empty()) {
        for (uint8_t add : controller.connected_devices) {
          printf("0x%x ", add);
        }
      } else {
        printf("No Devices connected \r\n");
      }

      printf("\r\n");
    #endif
  #endif

  while (true) {
    #ifdef CONTROLLER
      controller.update();
    #else
      leaf.update();
    #endif
  }
}
