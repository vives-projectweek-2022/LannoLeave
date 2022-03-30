#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>
#include <pico/multicore.h>

#include <commands.h>
#include <controller.h>
#include <helper_funcs_var.h>

#include <PicoLed.hpp>

#define VERSION 1

#define SDA 8
#define SCL 9
#define MOSI 0
#define MISO 3
#define CLK 2
#define CS 1

using namespace Lannooleaf;

volatile bool timer_triggered = false;

Controller controller(i2c0, SDA, SCL, MOSI, MISO, CLK, CS);

void set_alive_led(void) {
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
}

void spi_core(void) {
  Spi_slave spi_slave(0, 3, 2, 1);

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_leaf_led, [&](context* ctx) {

  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_all, [&](context* ctx){

  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::clear_leaf_led, [&](context* ctx){

  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::clear_all, [&](context* ctx){

  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_random, [&](context* ctx){
    // TODO: implement
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::version, [&](context* ctx){
    uint8_t version = VERSION;
    spi_slave.write_data(&version, 1);
  });

  printf("Handeling commands\n");

  while (true) {
    controller.c_command_handler.handel_command(spi_slave.read_command());
  }
}

int main() {
  stdio_init_all();
  set_alive_led();

  controller.device_discovery();
  controller.topology_discovery();

  multicore_launch_core1(spi_core);

  PRINT(controller.graph.to_string().c_str());
  PRINT("\n");

  // I think i²c will run on this core and spi on the other core
  while (true) {
    tight_loop_contents();
  }
}

