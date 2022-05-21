#include <array>
#include <memory>
#include <stdio.h>
#include <stdint.h>
#include <algorithm>

#include <pico/stdlib.h>
#include <pico/multicore.h>

#include <leaf.hpp>
#include <commands.hpp>
#include <controller.hpp>
#include <helper_funcs_var.hpp>

using namespace Lannooleaf;
using namespace Lannooleaf::interfaces;

std::unique_ptr< IUpdatable > unit = nullptr;

int main() {
  stdio_init_all();
  set_alive_led();

  for (select_pins pin : all_select_pins) {
    gpio_init(static_cast<uint> (pin));
    gpio_set_dir(static_cast<uint> (pin), GPIO_IN);
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
  while (!unit) {
    uint32_t status = gpio_get_all();

    try {

      if (!(status & 1 << 1)) {
        unit = std::unique_ptr< IUpdatable > (new Controller);
        PRINT("Controller initialized\n");
      }

      if (status & gpio_mask) {
        unit = std::unique_ptr< IUpdatable > (new Leaf);
        PRINT("Leaf initialized\n");
      }

    } catch (std::runtime_error& e) {
      printf(e.what());
      error_blink();
    }

  }

  PRINT("Stating update loop\n");

  while (true)
    unit->update();
  
}
