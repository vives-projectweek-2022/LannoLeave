#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>

#include <commands.h>
#include <controller.h>
#include <helper_funcs_var.h>

#include <PicoLed.hpp>

using namespace LannoLeaf;

Controller controller(i2c0);

volatile bool check = false;

bool repeating_timer_callback(struct repeating_timer *t) {
  check = true;
  return true;
}

void set_alive_led(void) {
  #ifdef DEBUG
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
  #endif
}

void add_packet_handlers(void) {
  controller.add_packet_handel(send_adj_list, [&](){

  });

  controller.add_packet_handel(set_leaf_led, [&](){
    
  });

  controller.add_packet_handel(set_leaf_all, [&](){
    // TODO: Implement
  });

  controller.add_packet_handel(set_unit_all, [&](){

  });

  controller.add_packet_handel(set_all_all, [&](){
    // TODO: Implement
  });

  controller.add_packet_handel(clear_leaf, [&](){
    // TODO: Implement
  });

  controller.add_packet_handel(clear_unit, [&](){
    // TODO: Implement
  });

  controller.add_packet_handel(clear_all, [&](){
    // TODO: Implement
  });

  controller.add_packet_handel(set_random, [&](){
    // TODO: Implement
  });

}

int main() {
  stdio_init_all();
  set_alive_led();

  sleep_ms(500);

  controller.device_discovery();
  controller.topology_discovery();

  add_packet_handlers();

  struct repeating_timer timer;

  add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);

  controller.ledstrip.fill(PicoLed::RGBW(0xff, 0xff, 0xff, 0xff));
  controller.ledstrip.show();

  // controller.ledstrip.fillRainbow(0x12, 0x13);
  // controller.ledstrip.show();

  uint8_t position = 0;
  uint8_t sleep_time = 100;
  uint8_t orange = 0;

  while (true) {
    if (check) {
      for (std::map <uint8_t, Node*>::iterator itr = controller.graph.map.begin(); itr != controller.graph.map.end(); itr++) {
        if (itr -> second -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) continue;
        controller.leaf_master.send_slave_message(itr -> second -> i2c_address, {
          slave_ping,
          0,
          { }
        });

        controller.leaf_master.get_slave_data(itr -> second -> i2c_address, 2);

        if (controller.leaf_master.memory[0] != 0xA5 && controller.leaf_master.memory[1] != 0x5A) {
          printf("Resetting \r\n");
          controller.reset();
        }
      }

      check = false;
    }

    uint8_t cmd;
    controller.handel_packet((bl_commands) cmd);

    position++;

    if (orange == 0) {
      controller.ledstrip.setPixelColor(position, PicoLed::RGB(0xff, 0, 0));
    } else if (orange == 1) {
      controller.ledstrip.setPixelColor(position, PicoLed::RGB(0, 0xff, 0));
    } else {
      controller.ledstrip.setPixelColor(position, PicoLed::RGB(0, 0, 0xff));
    }   
    
    controller.ledstrip.show();

    if (position == LED_LENGTH) {
      position = 0;
      orange++;
      if (orange == 3) orange = 0;
    }

    sleep_ms(20);
  }
}
