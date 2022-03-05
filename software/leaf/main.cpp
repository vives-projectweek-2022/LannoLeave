#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>
#include <commands.h>
#include <helper_funcs_var.h>
#include <leaf.h>

using namespace LannoLeaf;

Leaf leaf((uint8_t) UNCONFIGUREDADDRESS, i2c0);

void set_alive_led(void) {
  #ifdef DEBUG
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
  #endif
}

int main() {
  stdio_init_all();
  set_alive_led();

  leaf.add_command_handel(slave_set_i2c_address, [&](context* contx, msg_buff* msg){
    if (leaf.address() == 0x08) {
      leaf.address(msg -> buffer[0]);
    }
  });

  leaf.add_command_handel(slave_ping, [&](context* contx, msg_buff* msg){
    contx -> mem[0] = 0xA5;
    contx -> mem[1] = 0x5A;
  });

  leaf.add_command_handel(slave_reset_mem_counter, [&](context* contx, msg_buff* msg){
    contx -> mem_address = 0;
  });  

  leaf.add_command_handel(slave_set_sel_pin, [&](context* contx, msg_buff* msg){
    if (gpio_is_dir_out(msg -> buffer[0]) && !msg -> buffer[1]) {
      gpio_put(msg -> buffer[0], false);
      gpio_set_dir(msg -> buffer[0], GPIO_IN);
    } else {
      gpio_set_dir(msg -> buffer[0], GPIO_OUT);
      gpio_put(msg -> buffer[0], true);
    }
  });

  leaf.add_command_handel(slave_get_sel_pin, [&](context* contx, msg_buff* msg){
    contx -> mem[0] = leaf.sel_pin_status();
  });

  leaf.add_command_handel(slave_reset, [&](context* contx, msg_buff* msg){
    printf("Resetting\n");
    leaf.reset();
  });

  while (true) {
    while (!leaf.configured()) {
      leaf.update();
      if (leaf.sel_pin_status() && !leaf.slave_initialized()) leaf.slave_init();
    }

    while (leaf.configured()) {
      leaf.update();
    }
  }
}
