#include <array>
#include <memory>
#include <stdio.h>
#include <stdint.h>
#include <algorithm>

#include <pico/stdlib.h>

#include <leaf.hpp>
#include <commands.hpp>
#include <controller.hpp>
#include <helper_funcs_var.hpp>

using namespace Lannooleaf;

std::unique_ptr<Controller> controller = nullptr;
std::unique_ptr<Leaf> leaf = nullptr;


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

  // Wait untill unit initialized as controller or leaf
  while (!leaf && !controller) {
    uint32_t status = gpio_get_all();

    try {
      if (!(status & 1 << 1)) {
        controller = std::unique_ptr<Controller>(new Controller(i2c0, SDA, SCL));
        PRINT("Controller initialized\n");
      }

      if (status & gpio_mask) {
        leaf = std::unique_ptr<Leaf> (new Leaf);
        PRINT("Leaf initialized\n");
      }
    } 
    catch (std::runtime_error& e) {
      printf(e.what());
      while (true) error_blink();
    }

  }

  PRINT("Stating update loop\n");

  if (controller) {
    while (true) 
      controller->update();
  } else {
    while (true)
      leaf->update();
  }
}
