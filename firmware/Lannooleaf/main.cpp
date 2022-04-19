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

  set_irqs(true);

  // Wait untill initialized as controller or leaf
  while (!controller && !leaf) tight_loop_contents();

  // Controller is initialized
  if (controller) {
    Spi_slave::initialize(MOSI, MISO, CLK, CS);

    printf("Device discovery\n");
    controller->device_discovery();
    controller->topology_discovery();

    printf("%s\n", controller->graph.to_string().c_str());

    const uint8_t message = (uint8_t)slave_commands::discovery_done;
    controller->leaf_master.send_data(GENCALLADR, &message, 1);

    discover_animation(&controller->ledstrip, {0, 50, 100});

    while (true) {
      uint8_t cmd = Spi_slave::pop();
      if (cmd != 0xa5) commandHandler.handel_command(cmd);
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

void irq_init(uint gpio, uint32_t events) {
  // Disabling irq no longer needed
  set_irqs(false);

  printf("IRQ: %s\n", events & GPIO_IRQ_EDGE_FALL ? "FALL CNTRL" : "RISE LEAF");

  // Check where irq came from and intitialze Controller or Leaf
  if (events & GPIO_IRQ_EDGE_FALL) {
    controller = new Controller(i2c0, SDA, SCL);
    controller->add_controller_handlers(&commandHandler);
  } else {
    leaf = new Leaf;
    leaf->add_leaf_handlers(&commandHandler);
  }
}

void set_irqs(bool on) {
  gpio_set_irq_enabled_with_callback(CS, GPIO_IRQ_EDGE_FALL, on, irq_init);

  for (select_pins pin : all_select_pins)
    gpio_set_irq_enabled_with_callback((uint)pin, GPIO_IRQ_EDGE_RISE, on, irq_init);
}
