#include <array>
#include <stdio.h>
#include <stdint.h>
#include <algorithm>

#include <pico/stdlib.h>
#include <pico/multicore.h>

#include <leaf.h>
#include <commands.h>
#include <spi_slave.h>
#include <i2c_slave.h>
#include <controller.h>
#include <command_handler.h>
#include <helper_funcs_var.h>

#include <PicoLed.hpp>

using namespace Lannooleaf;

CommandHandler commandHandler;

Controller* controller = nullptr;
Leaf* leaf = nullptr;

void set_irqs(bool on);
void irq_init(uint gpio, uint32_t events);

int main() {
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
      controller = new Controller(i2c0, SDA, SCL);
      controller->add_controller_handlers(&commandHandler);
    } 
    if (status & gpio_mask) {
      leaf = new Leaf;
      leaf->add_leaf_handlers(&commandHandler);
    }
  };

  // Controller is initialized
  if (controller) {
    sleep_ms(2000);
    Spi_slave::initialize(MOSI, MISO, CLK, CS);

    printf("Device discovery\n");
    controller->device_discovery();
    controller->topology_discovery();

    printf("%s\n", controller->graph.to_string().c_str());

    for (auto [address, node] : controller->graph.map) {
      const uint8_t message = (uint8_t)slave_commands::discovery_done;
      controller->leaf_master.send_data(address, &message, 1);

      sleep_ms(500);
    }

    discover_animation(&controller->ledstrip, {0, 50, 100});

    while (true) {
      if (!Spi_slave::empty()) {
        uint8_t cmd = Spi_slave::pop();
        if (cmd == 0xa5) continue; 
        commandHandler.handel_command(cmd);
        printf("handeling command: 0x%02x\n", cmd);
      }
    };
  }

  // Leaf is initialized
  else {
    I2c_slave::initialize(UNCONFIGUREDADDRESS, i2c0, SDA, SCL);

    while (true) {
      leaf->update();
      if (!I2c_slave::empty()) {
        uint8_t cmd = I2c_slave::pop();
        commandHandler.handel_command(cmd);
      }
    };
  }
}
