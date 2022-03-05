#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>
#include <commands.h>
#include <helper_funcs_var.h>
#include <controller.h>

using namespace LannoLeaf;

Controller controller(i2c0);

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

  #ifdef DEBUG
  auto t1 = get_absolute_time();
  #endif

  PRINT("Starting device discovery\r\n");
  controller.device_discovery();
  
  PRINT("Starting topo discovery\r\n");
  controller.topology_discovery();
  
  #ifdef DEBUG
  auto t2 = get_absolute_time();
  #endif

  PRINT_A("Algorithms took: %lld µs\r\n", absolute_time_diff_us(t1, t2));

  while (true) {
    if (uint8_t lenght = uart_is_readable(uart0)) {
      uint8_t buffer[10];
      uart_read_blocking(uart0, buffer, lenght);
      
      if (buffer[0] == 0x61) {
        uart_write_blocking(uart0, (const uint8_t *)controller.graph.to_string().c_str(), controller.graph.to_string().size());
      } else if (buffer[0] == 0x01) {
        controller.reset();
      }
    }
  }
}