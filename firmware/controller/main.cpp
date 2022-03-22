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
    // TODO: Implement
  });

  controller.add_packet_handel(set_leaf_led, [&](){
    // TODO: Implement  
  });

  controller.add_packet_handel(set_leaf_all, [&](){
    // TODO: Implement
  });

  controller.add_packet_handel(set_unit_all, [&](){
    // TODO: handel addressing

    uint8_t buffer[6];
    controller.command_handler->read_data(buffer, 6);
    printf("RGBW: %i, %i, %i, %i\n", buffer[2], buffer[3], buffer[4], buffer[5]);
    controller.ledstrip.fill(PicoLed::RGBW(buffer[2], buffer[3], buffer[4], buffer[5]));
    controller.ledstrip.show();
  });

  controller.add_packet_handel(set_all_all, [&](){
    uint8_t buffer[4];
    controller.command_handler->read_data(buffer, 4);

    controller.leaf_master.send_slave_message(GENCALLADR, {
      slave_set_all_led,
      4,
      { buffer[0], buffer[1], buffer[2], buffer[3] }
    });

    controller.ledstrip.fill(PicoLed::RGBW(buffer[0], buffer[1], buffer[2], buffer[3]));
    controller.ledstrip.show();
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

  sleep_ms(2000);

  controller.device_discovery();
  controller.topology_discovery();

  add_packet_handlers();

  struct repeating_timer timer;

  add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);

  printf(controller.graph.to_string().c_str());
  printf("\n");

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

    if (uint8_t cmd = controller.command_handler->read_command()) {
      printf("Handel packet\n");
      controller.handel_packet((m_commands)cmd);
    }
  }
}
