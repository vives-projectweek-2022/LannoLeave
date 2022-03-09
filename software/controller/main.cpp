#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>

#include <commands.h>
#include <helper_funcs_var.h>
#include <controller.h>
#include <PicoLed.hpp>

using namespace LannoLeaf;

Controller controller(i2c0, i2c1);

void set_alive_led(void) {
  #ifdef DEBUG
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
  #endif
}

int main() {
  stdio_init_all();
  set_alive_led();

  sleep_ms(500);

  controller.device_discovery();
  controller.topology_discovery();

  while (true) {
    // Uart packets
    // 1. Start byte
    // 2. Length
    // 3. Address
    // 4. Sub address
    // 5. Command
    // 6. extra data (RGB, Brightness, etc.)

    if (uint8_t lenght = uart_is_readable(uart0)) {
      uint8_t buffer[10];
      uart_read_blocking(uart0, buffer, lenght);
      
      if (buffer[0] == start_read) {
        uart_read_blocking(uart0, buffer, 1);
        uart_read_blocking(uart0, buffer, buffer[0]);

        

      }
    }
  }
}
