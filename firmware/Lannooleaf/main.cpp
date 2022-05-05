#include <array>
#include <memory>
#include <stdio.h>
#include <stdint.h>
#include <algorithm>

#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <hardware/watchdog.h>

#include <leaf.h>
#include <commands.h>
#include <spi_slave.h>
#include <i2c_slave.h>
#include <controller.h>
#include <command_handler.h>
#include <helper_funcs_var.h>

using namespace Lannooleaf;

CommandHandler commandHandler;

std::unique_ptr<Controller> controller;
std::unique_ptr<Leaf> leaf;

void spi_core(void) {
  SPISlave::initialize(MOSI, MISO, CLK, CS);

  while (true) tight_loop_contents();
}

void leaf_core(void) {
  I2CSlave::initialize(UNCONFIGUREDADDRESS, i2c0, SDA, SCL);

  while (true) tight_loop_contents();
}

int main() {
  // set_sys_clock_khz(250000, true); // Overclockinggg xD

  stdio_init_all();
  set_alive_led();

  for (select_pins pin : all_select_pins) {
    gpio_init((uint)pin);
    gpio_set_dir((uint)pin, GPIO_IN);
  }

  gpio_init(CS);
  gpio_set_dir(CS, GPIO_IN);
  gpio_pull_up(CS);

  gpio_pull_up(SDA);
  gpio_pull_up(SCL);

  uint32_t gpio_mask = 0;
  for (auto pin : all_select_pins) {
    gpio_mask |= 1 << (uint)pin; 
  }

  // Wait untill initialized as controller or leaf
  while (!controller && !leaf) {
    uint32_t status = gpio_get_all();
    if (!(status & 1 << 1)) {
      controller = std::unique_ptr<Controller>(new Controller(i2c0, SDA, SCL));
      uint8_t data[8] = {0xff};
      controller->leaf_master.send_data(GENCALLADR,data, 8);

      controller->add_controller_handlers(&commandHandler);
    } 
    if (status & gpio_mask) {
      leaf = std::unique_ptr<Leaf> (new Leaf);
      leaf->add_leaf_handlers(&commandHandler);
    }
  };

  // Controller is initialized
  if (controller) {

    multicore_launch_core1(spi_core);

    controller->device_discovery();
    controller->topology_discovery();

    for (auto [address, node] : controller->graph.map) {
      const uint8_t message = (uint8_t)slave_commands::discovery_done;
      controller->leaf_master.send_data(address, &message, 1);

      sleep_ms(500);
    }

    discover_animation(&controller->ledstrip, {0, 50, 100});

    while (true) {
      uint8_t cmd = SPISlave::pop();
      if (cmd != 0xa5 && cmd != 0x00) {
        if (!commandHandler.handel_command(cmd)){
          SPISlave::reset();
        }
      }
    };
  }

  // Leaf is initialized
  else {
    multicore_launch_core1(leaf_core);

    while (true) {
      leaf->update();
      uint8_t cmd = I2CSlave::pop();
      commandHandler.handel_command(cmd);
    };
  }
}
