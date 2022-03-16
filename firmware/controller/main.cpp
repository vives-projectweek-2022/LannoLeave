#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>

#include <commands.h>
#include <controller.h>
#include <helper_funcs_var.h>

#include <PicoLed.hpp>

using namespace LannoLeaf;

Controller controller(i2c0);

volatile bool check = false;

bool repeating_timer_callback(struct repeating_timer *t) {
  printf("Timer\r\n");
  check = true;
  return true;
}

void set_alive_led(void) {
  #ifdef DEBUG
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
  #endif
}

void add_packet_handlers(void) {

  controller.add_packet_handel(send_adj_list, [&](){
    uart_write_blocking(uart0, (const uint8_t *) controller.graph.to_string().c_str(), controller.graph.to_string().size());
  });

  controller.add_packet_handel(set_leaf_led, [&](){
    uint8_t buffer[6];
    uart_read_blocking(uart0, buffer, 6);

    if (buffer[0] == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
      controller.ledstrip.setPixelColor(buffer[1], PicoLed::RGBW(buffer[2], buffer[3], buffer[4], buffer[5]));
    } else {
      controller.leaf_master.send_slave_message(buffer[0], {
        slave_set_led,
        5,
        { buffer[1], buffer[2], buffer[3], buffer[4], buffer[5] }
      });
    }
  });

  controller.add_packet_handel(set_leaf_all, [&](){
    // TODO: Implement
  });

  controller.add_packet_handel(set_unit_all, [&](){
    uint8_t buffer[5];
    uart_read_blocking(uart0, buffer, 5);

    if (buffer[0] == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
      controller.ledstrip.fill(PicoLed::RGBW(buffer[1], buffer[2], buffer[3], buffer[4]));
    } else {
      controller.leaf_master.send_slave_message(buffer[0], {
        slave_set_all_led,
        4,
        { buffer[1], buffer[2], buffer[3], buffer[4] }
      });
    }
  });

  controller.add_packet_handel(set_all_all, [&](){
    // TODO: Implement
  });

  controller.add_packet_handel(clear_leaf, [&](){
    // TODO: Implement
  });

  controller.add_packet_handel(clear_unit, [&](){
    // TODO: Implement
  });

  controller.add_packet_handel(clear_all, [&](){
    // TODO: Implement
  });

  controller.add_packet_handel(set_random, [&](){
    // TODO: Implement
  });

}

int main() {
  stdio_init_all();
  set_alive_led();

  sleep_ms(500);

  controller.device_discovery();
  controller.topology_discovery();

  add_packet_handlers();

  struct repeating_timer timer;

  add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);

  while (true) {
    if (check) {
      for (std::map <uint8_t, Node*>::iterator itr = controller.graph.map.begin(); itr != controller.graph.map.end(); itr++) {
        if (itr -> second -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) continue;
        controller.leaf_master.send_slave_message(itr -> second -> i2c_address, {
          slave_ping,
          0,
          { }
        });

        controller.leaf_master.get_slave_data(itr -> second -> i2c_address, 2);

        if (controller.leaf_master.memory[0] != 0xA5 && controller.leaf_master.memory[1] != 0x5A) {
          printf("Resetting \r\n");
          controller.reset();
        }
      }

      check = false;
    }

    if (uart_is_readable(uart0)) {
      printf("Reading message \r\n");
      uint8_t cmd;
      uart_read_blocking(uart0, &cmd, 1);

      controller.handel_packet((bl_commands) cmd);
    }
  }
}
