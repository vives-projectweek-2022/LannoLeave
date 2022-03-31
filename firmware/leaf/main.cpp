#include <stdio.h>
#include <stdint.h>

#include <leaf.h>
#include <commands.h>
#include <helper_funcs_var.h>

#include <pico/stdlib.h>

using namespace Lannooleaf;

Leaf leaf((uint8_t) UNCONFIGUREDADDRESS, i2c0, 8, 9);

void set_alive_led(void) {
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
}

void add_handlers(void) {
  leaf.l_command_handler.add_handler((uint8_t)slave_commands::slave_set_i2c_address, [&](context* ctx){
    if (leaf.address() == 0x08) {
      leaf.address(ctx -> read_mem -> memory[0]);
    }
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::slave_ping, [&](context* ctx){
    ctx -> write_mem -> memory[0] = 0xA5;
    ctx -> write_mem -> memory[1] = 0x5A;
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::slave_reset_mem_counter, [&](context* ctx){
    ctx -> write_mem -> memory_address = 0;
  });  

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::slave_set_sel_pin, [&](context* ctx){
    if (gpio_is_dir_out(ctx -> read_mem -> memory[0]) && !ctx -> read_mem -> memory[1]) {
      gpio_put(ctx -> read_mem -> memory[0], false);
      gpio_set_dir(ctx -> read_mem ->  memory[0], GPIO_IN);
    } else {
      gpio_set_dir(ctx -> read_mem ->  memory[0], GPIO_OUT);
      gpio_put(ctx -> read_mem ->  memory[0], true);
    }
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::slave_get_sel_pin, [&](context* ctx){
    ctx -> write_mem -> memory[0] = leaf.sel_pin_status();
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::slave_reset, [&](context* ctx){
    leaf.reset();
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::slave_is_neighbor, [&](context* ctx){
    if (leaf.sel_pin_status()) {
      ctx -> write_mem -> memory_address++;
      ctx -> write_mem -> memory[ctx -> write_mem -> memory_address] = ctx -> read_mem ->  memory[0];
      ctx -> write_mem -> memory_address++;
      ctx -> write_mem -> memory[ctx -> write_mem -> memory_address] = leaf.sel_pin_status();
    }
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::slave_neighbor_size, [&](context* ctx){
    ctx -> write_mem -> memory[0] = (ctx -> write_mem -> memory_address / 2);
    ctx -> write_mem -> memory_address = 0;
  });

  // LED commando's
  leaf.l_command_handler.add_handler((uint8_t)slave_commands::slave_set_led, [&](context* ctx){
    leaf.ledstrip.setPixelColor(ctx->read_mem->memory[0], PicoLed::RGB(ctx->read_mem->memory[1], ctx->read_mem->memory[2], ctx->read_mem->memory[3]));
    leaf.ledstrip.show();
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::slave_set_all_led, [&](context* ctx){
    leaf.ledstrip.fill(PicoLed::RGB(ctx->read_mem->memory[0], ctx->read_mem->memory[1], ctx->read_mem->memory[2]));
    leaf.ledstrip.show();
  });
}

int main() {
  stdio_init_all();
  set_alive_led();

  add_handlers();

  while (true) {
    while (!leaf.configured()) {
      leaf.update();
      if (leaf.sel_pin_status() && !leaf.slave_initialized()) {
        leaf.slave_init();
      } 
    }

    while (leaf.configured()) {
      leaf.update();
    }
  }
}
