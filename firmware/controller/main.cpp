#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>
#include <pico/multicore.h>

#include <commands.h>
#include <controller.h>
#include <helper_funcs_var.h>
#include <spi_slave.h>

#include <PicoLed.hpp>

#define VERSION 1

#define SDA 8
#define SCL 9
#define MOSI 0
#define MISO 3
#define CLK 2
#define CS 1

using namespace Lannooleaf;

volatile bool timer_triggered = false;

Controller controller(i2c0, SDA, SCL, MOSI, MISO, CLK, CS);

void set_alive_led(void) {
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
}

void spi_core(void) {

  // Initialze spi on core 1 to handel interupts on core 1
  Spi_slave::Get().initialize(0, 3, 2, 1);

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_leaf_led, [&](context* ctx) {
    const Packet& pkt = Spi_slave::Get().fifo.front();

    if (pkt.read_buffer[1] == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
      controller.ledstrip.setPixelColor(pkt.read_buffer[2], PicoLed::RGB(pkt.read_buffer[3], pkt.read_buffer[4], pkt.read_buffer[5]));
      controller.ledstrip.show();
    } else {
      controller.leaf_master.send_slave_message(pkt.read_buffer[1], {
        (uint8_t)slave_commands::slave_set_led,
        4,
        {pkt.read_buffer[2], pkt.read_buffer[3], pkt.read_buffer[4], pkt.read_buffer[5]}
      });
    }
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_all, [&](context* ctx){
    const Packet& pkt = Spi_slave::Get().fifo.front();
    
    controller.leaf_master.send_slave_message(GENCALLADR, {
      (uint8_t)slave_commands::slave_set_all_led,
      3,
      { pkt.read_buffer[1], pkt.read_buffer[2], pkt.read_buffer[3] }
    });

    controller.ledstrip.fill(PicoLed::RGB(pkt.read_buffer[1], pkt.read_buffer[2], pkt.read_buffer[3]));
    controller.ledstrip.show();
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::clear_leaf_led, [&](context* ctx){

  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::clear_all, [&](context* ctx){

  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_random, [&](context* ctx){
    // TODO: implement
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::version, [&](context* ctx){
    uint8_t version = VERSION;
  });

  while (true) {
    tight_loop_contents();
  }
}

int main() {
  stdio_init_all();
  set_alive_led();

  controller.device_discovery();
  controller.topology_discovery();

  PRINT(controller.graph.to_string().c_str());
  PRINT("\n");

  PRINT("Launcing core 1\n");
  multicore_launch_core1(spi_core);

  while (true) {
    // Handel incomming commands from core 0
    if (!Spi_slave::Get().fifo.empty()) {
      printf("Command to execute: 0x%02x\n", Spi_slave::Get().fifo.front().read_buffer[0]);
      controller.c_command_handler.handel_command(Spi_slave::Get().fifo.front().read_buffer[0]);
      Spi_slave::Get().fifo.pop();
    } 
  }
}

