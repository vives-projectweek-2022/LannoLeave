#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>

#include <commands.h>
#include <controller.h>
#include <helper_funcs_var.h>

#include <PicoLed.hpp>

#define SDA 8
#define SCL 9
#define MOSI 0
#define MISO 3
#define CLK 2
#define CS 1

using namespace LannoLeaf;

volatile bool timer_triggered = false;

Controller controller(i2c0, SDA, SCL, MOSI, MISO, CLK, CS);

bool repeating_timer_callback(struct repeating_timer *t) {
  PRINT("TIMER\n");
  if (!timer_triggered) timer_triggered = true;
  return true;
}

void set_alive_led(void) {
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
}

void add_handlers(void) {
  controller.c_command_handler.add_handler((uint8_t)controller_commands::send_adj_list, [&](context* ctx){
    // TODO: Implement
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_leaf_led, [&](context* ctx){
    // TODO: Implement  
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_leaf_all, [&](context* ctx){
    // TODO: Implement
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_unit_all, [&](context* ctx){
    // TODO: handel addressing

    uint8_t buffer[6];
    controller.c_spi_slave.read_data(buffer, 6);
    controller.ledstrip.fill(PicoLed::RGB(buffer[2], buffer[3], buffer[4]));
    controller.ledstrip.show();
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_all_all, [&](context* ctx){
    uint8_t buffer[4];
    controller.c_spi_slave.read_data(buffer, 4);

    controller.leaf_master.send_slave_message(GENCALLADR, {
      (uint8_t)slave_commands::slave_set_all_led,
      4,
      { buffer[0], buffer[1], buffer[2], buffer[3] }
    });

    controller.ledstrip.fill(PicoLed::RGB(buffer[0], buffer[1], buffer[2]));
    controller.ledstrip.show();
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::clear_leaf, [&](context* ctx){
    // TODO: Implement
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::clear_unit, [&](context* ctx){
    // TODO: Implement
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::clear_all, [&](context* ctx){
    // TODO: Implement
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_random, [&](context* ctx){
    // TODO: Implement
  });

}

void scan_devices(void) {
  for (std::map <uint8_t, Node*>::iterator itr = controller.graph.map.begin(); itr != controller.graph.map.end(); itr++) {
    if (itr -> second -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) continue;
    controller.leaf_master.send_slave_message(itr -> second -> i2c_address, {
      (uint8_t)slave_commands::slave_ping,
      0,
      { }
    });

    controller.leaf_master.get_slave_data(itr -> second -> i2c_address, 2);

    if (controller.leaf_master.memory[0] != 0xA5 && controller.leaf_master.memory[1] != 0x5A) {
      PRINT("Resetting!\n");
      controller.reset();
    }
  }

  timer_triggered = false;
}

int main() {
  stdio_init_all();
  set_alive_led();

  sleep_ms(2000);

  controller.device_discovery();
  controller.topology_discovery();

  add_handlers();

  struct repeating_timer timer;

  add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);

  PRINT(controller.graph.to_string().c_str());
  PRINT("\n");

  PRINT(controller.graph.node_to_coords().c_str());
  PRINT("\n");

  while (true) {
    if (timer_triggered) scan_devices();
    if (uint8_t cmd = controller.c_spi_slave.read_command()) controller.c_command_handler.handel_command(cmd);
  }
}

