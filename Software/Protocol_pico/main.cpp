#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"

#include "src/leaf.h"
#include "src/controller.h"
#include "src/commands.h"

#define COaNTROLLER
#define I2CADDRESS 0x34

#ifndef CONTROLLER
  LannoLeaf::Leaf leaf((uint8_t) I2CADDRESS);
#else
  LannoLeaf::Controller controller;
#endif

void set_alive_led(void) {
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
}

int main() {
  stdio_init_all();
  set_alive_led();

  sleep_ms(5000);
  printf("Slept for 5 sec \r\n");
  
  // printf("Scanniing for i2c devices\r\n");
  // controller.scan_i2c_devices();

  // if (!controller.connected_devices.empty()) {
  //   for (uint8_t add : controller.connected_devices) {
  //     printf("0x%x ", add);
  //   }
  // } else {
  //   printf("No Devices connected \r\n");
  // }

  // printf("\r\n");

  while (true) {
    #ifdef CONTROLLER
      sleep_ms(2000);
      controller.send_slave_message(I2CADDRESS, LannoLeaf::slave_detect_message);
    #else
      leaf.update();
    #endif
  }
}
