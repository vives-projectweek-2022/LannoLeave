#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>
#include <commands.h>
#include <helper_funcs_var.h>
#include <leaf.h>

using namespace LannoLeaf;

Leaf leaf((uint8_t) UNCONFIGUREDADDRESS, i2c0);

void set_alive_led(void) {
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
}

void add_handlers(void) {
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
    leaf.reset();
  });

  leaf.add_command_handel(slave_is_neighbor, [&](context* contx, msg_buff* msg){
    if (leaf.sel_pin_status()) {
      PRINT("Is neighbor!\n");
      contx -> mem_address++;
      contx -> mem[contx -> mem_address] = msg -> buffer[0];
      
      PRINT_A("Neigbor = %02x\n", msg -> buffer[0]);

      contx -> mem_address++;
      contx -> mem[contx -> mem_address] = leaf.sel_pin_status();
    } 
  });

  leaf.add_command_handel(slave_neighbor_size, [&](context* contx, msg_buff* msg){
    contx -> mem[0] = (contx -> mem_address / 2);
    contx -> mem_address = 0;
  });

  leaf.add_command_handel(slave_set_led, [&](context* contx, msg_buff* msg){
    leaf.ledstrip.setPixelColor(msg -> buffer[0], PicoLed::RGBW(msg -> buffer[1], msg -> buffer[2], msg -> buffer[3], msg -> buffer[4]));
    leaf.ledstrip.show();
  });

  leaf.add_command_handel(slave_set_all_led, [&](context* contx, msg_buff* msg){
    leaf.ledstrip.fill(PicoLed::RGBW(msg -> buffer[0], msg -> buffer[1], msg -> buffer[2], msg -> buffer[3]));
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

      if (gpio_get(15))  {
        printf("pin status: 0x%02x\n", gpio_get(15));
        printf("Sel_pin_status: 0x%02x\n", leaf.sel_pin_status());
      }
    }

    printf("Initialized\n");

    while (leaf.configured()) {
      leaf.update();
    }
  }
}
