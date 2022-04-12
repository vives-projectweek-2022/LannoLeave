#include <array>
#include <stdio.h>
#include <stdint.h>

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
void set_alive_led(void);
void irq_init(uint gpio, uint32_t events);

int main() {
  // Setting system clock to 250MHz
  set_sys_clock_khz(250000, true); //! If things start breaking lower this

  stdio_init_all();
  set_alive_led();

  set_irqs(true);

  // Wait untill initialized as controller or leaf
  while (!controller && !leaf) tight_loop_contents();

  // Controller is initialized
  if (controller) {
    Spi_slave::initialize(MOSI, MISO, CLK, CS);

    controller->device_discovery();
    controller->topology_discovery();

    PRINT_A("%s\n", controller->graph.to_string().c_str());

    while (true) {
      if (!Spi_slave::empty()) {
        uint8_t cmd = Spi_slave::pop();
        if (cmd != 0xa5) commandHandler.handel_command(cmd);
      }
    };
  } 
  
  // Leaf is initialized
  else {
    printf("Configured as slave\n");
    I2c_slave::initialize(UNCONFIGUREDADDRESS, i2c0, SDA, SCL);
    
    while (true) {
      if (!I2c_slave::empty()) {
        leaf->update();
        uint8_t cmd = I2c_slave::pop();
        printf("Handleing command 0x%02x\n", cmd);
        commandHandler.handel_command(cmd);
      }
    }
  }
}

void set_alive_led(void) {
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
}

void irq_init(uint gpio, uint32_t events) {
  set_irqs(false);

  if (events & GPIO_IRQ_EDGE_FALL) {
    controller = new Controller(i2c0, SDA, SCL);
    controller->add_controller_handlers(&commandHandler);
  } else {
    leaf = new Leaf;
    leaf->add_leaf_handlers(&commandHandler);
  }
}

void set_irqs(bool on) {
  gpio_init(CS);
  gpio_set_dir(CS, GPIO_IN);

  gpio_set_irq_enabled_with_callback(CS, GPIO_IRQ_EDGE_FALL, on, irq_init);

  for (select_pins pin : all_select_pins) {
    gpio_init((uint)pin);
    gpio_set_dir((uint)pin, GPIO_IN);
    gpio_set_irq_enabled_with_callback((uint)pin, GPIO_IRQ_EDGE_RISE, on, irq_init);
  }
}
